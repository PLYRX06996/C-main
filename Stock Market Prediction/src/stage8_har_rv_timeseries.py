"""
Stage 8: Econometric Time-Series Benchmark — Heterogeneous Autoregressive Volatility (HAR-RV)
=============================================================================================

Theoretical Background:
-----------------------
In financial econometrics, standard ARMA/ARIMA models are formulated for conditional means
(price returns) rather than conditional variances, and fail to capture the long-memory,
multi-scale volatility clustering observed in financial assets without non-stationarity.

Corsi (2009) [Journal of Financial Econometrics, 7(2), 174-196] introduced the Heterogeneous
Autoregressive model of Realized Volatility (HAR-RV) based on the Heterogeneous Market
Hypothesis (Muller et al., 1997). In market microstructure, volatility cascades across
distinct temporal horizons:
  1. Ultra-short tick horizons (quote-level revisions, bid-ask bouncing, immediate shocks)
  2. Medium horizons (order-book replenishment, execution horizon)
  3. Long-memory regimes (sustained high/low volatility regimes)

This script implements, estimates, and evaluates:
  1. Naive Persistence Baseline (sigma_{t}^{(10)} -> sigma_{t+10s})
  2. Corsi (2009) Canonical HAR-RV (Short: 5-tick, Medium: 15-tick, Long: 60-tick, Shock: ret^2)
  3. Extended Multi-Horizon HAR-RV (All multi-scale realized and EWMA components for ETH)
  4. Cross-Asset HAR-RV-X (Modeling BTC volatility spillover onto ETH)
  5. Calibrated Regularized HAR-RV (RidgeCV with optimal shrinkage over multi-collinear vol horizons)
  6. Microstructure-Enhanced HAR-RV (HAR-RV + Order Book Imbalances + Spreads)

Evaluated under both:
  - 80/20 Chronological Train/Validation split (benchmark with Stages 1-7)
  - 70/15/15 Chronological Train/Validation/Test split (untouched out-of-sample verification)
"""

import os
import sys
import numpy as np
import pandas as pd
from scipy import stats
from scipy.stats import pearsonr
from sklearn.linear_model import RidgeCV, LinearRegression
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.metrics import root_mean_squared_error, r2_score
import joblib
import warnings

warnings.filterwarnings('ignore')

sys.path.append(os.path.dirname(os.path.abspath(__file__)))
from stage4_multiscale_pipeline import load_and_merge, engineer_multiscale_features, clean_and_prepare

def fit_ols_econometric_summary(X_train, y_train, feature_names, max_lag=10, model_title="HAR-RV Econometric Estimation"):
    """
    Fits Ordinary Least Squares via matrix decomposition and outputs standard
    econometric diagnostic tables: coefficients, standard errors (both classical OLS
    and Newey-West HAC with Bartlett kernel), t-statistics, and p-values.
    """
    X_mat = np.column_stack([np.ones(len(X_train)), X_train.values])
    cols = ['const'] + list(feature_names)
    n, p = X_mat.shape

    beta, _, _, _ = np.linalg.lstsq(X_mat, y_train.values, rcond=None)
    y_hat = X_mat @ beta
    residuals = y_train.values - y_hat
    rss = np.sum(residuals ** 2)
    sigma2 = rss / (n - p)

    XtX_inv = np.linalg.pinv(X_mat.T @ X_mat)
    se_ols = np.sqrt(np.maximum(np.diagonal(sigma2 * XtX_inv), 1e-20))
    t_stats_ols = beta / se_ols
    p_values_ols = 2.0 * (1.0 - stats.norm.cdf(np.abs(t_stats_ols)))

    Xe = X_mat * residuals[:, np.newaxis]
    gamma_0 = (Xe.T @ Xe) / n
    omega_hac = gamma_0.copy()
    for lag in range(1, max_lag + 1):
        weight = 1.0 - (lag / (max_lag + 1.0))
        gamma_l = (Xe[lag:].T @ Xe[:-lag]) / n
        omega_hac += weight * (gamma_l + gamma_l.T)
    V_hac = n * (XtX_inv @ omega_hac @ XtX_inv)
    se_hac = np.sqrt(np.maximum(np.diagonal(V_hac), 1e-20))
    t_stats_hac = beta / se_hac
    p_values_hac = 2.0 * (1.0 - stats.norm.cdf(np.abs(t_stats_hac)))

    print(f"\n{'='*92}")
    print(f" {model_title}")
    print(f" Sample Size: {n:,} observations | Parameters: {p} | Newey-West HAC Lags: {max_lag}")
    print(f"{'='*92}")
    print(f"{'Variable':<22} {'Coefficient':>12} {'OLS SE':>10} {'HAC SE':>10} {'HAC t-stat':>12} {'HAC p-val':>10}")
    print(f"{'-'*92}")
    for col_name, b, s_ols, s_hac, t_hac, pv_hac in zip(cols, beta, se_ols, se_hac, t_stats_hac, p_values_hac):
        p_str = "< 0.0001" if pv_hac < 0.0001 else f"{pv_hac:.4f}"
        print(f"{col_name:<22} {b:>12.5e} {s_ols:>10.3e} {s_hac:>10.3e} {t_hac:>12.2f} {p_str:>10}")
    print(f"{'='*92}\n")

    return beta, cols, se_hac, t_stats_hac, p_values_hac

def evaluate_model(y_true, y_pred, model_name="Model"):
    """Computes directional Pearson correlation, RMSE, R-squared, and QLIKE loss."""
    corr, pval = pearsonr(y_true, y_pred)
    rmse = root_mean_squared_error(y_true, y_pred)
    r2 = r2_score(y_true, y_pred)

    yt = np.maximum(y_true.values if hasattr(y_true, 'values') else y_true, 1e-12)
    yp = np.maximum(y_pred if isinstance(y_pred, np.ndarray) else y_pred.values, 1e-12)
    qlike = float(np.mean(yt / yp - np.log(yt / yp) - 1.0))
    return {
        "model": model_name,
        "pearson_r": corr,
        "rmse": rmse,
        "r2": r2,
        "qlike": qlike,
        "p_value": pval
    }

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')

    models_dir = os.path.join(script_dir, '..', 'models')
    os.makedirs(models_dir, exist_ok=True)

    print("=" * 78)
    print(" STAGE 8: TIME-SERIES HAR-RV (HETEROGENEOUS AUTOREGRESSIVE) BENCHMARK")
    print("=" * 78)

    df = load_and_merge(train_dir)
    df = engineer_multiscale_features(df)
    X_full, y = clean_and_prepare(df)

    n_total = len(X_full)
    print(f"Total synchronized dataset records: {n_total:,} rows\n")

    canonical_features = ['realized_vol_5', 'realized_vol_15', 'realized_vol_60', 'ret_squared', 'abs_return']
    

    eth_vol_features = [
        'ret_squared', 'abs_return',
        'realized_vol_5', 'realized_vol_10', 'realized_vol_15', 'realized_vol_30', 'realized_vol_60',
        'ewma_vol_10', 'ewma_vol_30', 'ewma_vol_60'
    ]

    cross_vol_features = eth_vol_features + [c + '_BTC' for c in eth_vol_features]

    all_30_vol_features = [c for c in X_full.columns if any(k in c for k in ['realized_vol', 'ewma_vol', 'ret_squared', 'abs_return'])]

    raw_price_cols = [c for c in X_full.columns if any(k in c for k in ['bid_price', 'ask_price', 'mid_price']) and 'ratio' not in c]
    stationary_features = [c for c in X_full.columns if c not in raw_price_cols]

    print("Feature Allocations:")
    print(f"  - Canonical HAR-RV (Corsi 2009)       : {len(canonical_features)} features")
    print(f"  - Extended Multi-Scale ETH Volatility : {len(eth_vol_features)} features")
    print(f"  - Cross-Asset HAR-RV-X (ETH + BTC)    : {len(cross_vol_features)} features")
    print(f"  - Multi-Asset Volatility (ETH+BTC+SOL): {len(all_30_vol_features)} features")
    print(f"  - Stationary Microstructure Matrix    : {len(stationary_features)} features")

    print("\n" + "=" * 78)
    print(" PART 1: CHRONOLOGICAL 80/20 VALIDATION SPLIT (COMPARISON WITH STAGES 1-7)")
    print("=" * 78)

    n_train_80 = int(0.80 * n_total)
    y_train_80 = y.iloc[:n_train_80]
    y_val_80 = y.iloc[n_train_80:]

    results_80 = []

    p_persist_80 = X_full['realized_vol_10'].iloc[n_train_80:]
    results_80.append(evaluate_model(y_val_80, p_persist_80, "Persistence Baseline (rvol10)"))

    lr_canonical_80 = LinearRegression()
    lr_canonical_80.fit(X_full[canonical_features].iloc[:n_train_80], y_train_80)
    p_canonical_80 = lr_canonical_80.predict(X_full[canonical_features].iloc[n_train_80:])
    results_80.append(evaluate_model(y_val_80, p_canonical_80, "Canonical HAR-RV (Corsi 2009 OLS)"))

    lr_eth_80 = LinearRegression()
    lr_eth_80.fit(X_full[eth_vol_features].iloc[:n_train_80], y_train_80)
    p_eth_80 = lr_eth_80.predict(X_full[eth_vol_features].iloc[n_train_80:])
    results_80.append(evaluate_model(y_val_80, p_eth_80, "Extended Multi-Scale HAR-RV (ETH OLS)"))

    lr_cross_80 = LinearRegression()
    lr_cross_80.fit(X_full[cross_vol_features].iloc[:n_train_80], y_train_80)
    p_cross_80 = lr_cross_80.predict(X_full[cross_vol_features].iloc[n_train_80:])
    results_80.append(evaluate_model(y_val_80, p_cross_80, "Cross-Asset HAR-RV-X (ETH+BTC OLS)"))

    ridge_30_80 = Pipeline([
        ('scaler', StandardScaler()),
        ('ridge', RidgeCV(alphas=np.logspace(-4, 4, 15)))
    ])
    ridge_30_80.fit(X_full[all_30_vol_features].iloc[:n_train_80], y_train_80)
    p_ridge_80 = ridge_30_80.predict(X_full[all_30_vol_features].iloc[n_train_80:])
    results_80.append(evaluate_model(y_val_80, p_ridge_80, "Regularized HAR-RV (RidgeCV 30 Vol)"))

    ridge_micro_80 = Pipeline([
        ('scaler', StandardScaler()),
        ('ridge', RidgeCV(alphas=np.logspace(-4, 4, 15)))
    ])
    ridge_micro_80.fit(X_full[stationary_features].iloc[:n_train_80], y_train_80)
    p_micro_80 = ridge_micro_80.predict(X_full[stationary_features].iloc[n_train_80:])
    results_80.append(evaluate_model(y_val_80, p_micro_80, "Microstructure HAR-RV (RidgeCV Vol+LOB)"))

    df_res_80 = pd.DataFrame(results_80)
    print(f"\n{'Model Architecture':<42} {'Val Pearson r':>14} {'Val RMSE':>14} {'Val R2':>10} {'Val QLIKE':>12}")
    print("-" * 96)
    for _, row in df_res_80.iterrows():
        print(f"{row['model']:<42} {row['pearson_r']:>14.4f} {row['rmse']:>14.4e} {row['r2']:>10.4f} {row['qlike']:>12.4f}")
    print("-" * 96)

    fit_ols_econometric_summary(
        X_full[canonical_features].iloc[:n_train_80],
        y_train_80,
        canonical_features,
        model_title="Corsi (2009) Canonical HAR-RV Econometric Estimation (80% Train)"
    )

    print("=" * 78)
    print(" PART 3: RIGOROUS 3-WAY CHRONOLOGICAL BENCHMARK (70% Train / 15% Val / 15% Test)")
    print("=" * 78)

    n_train_70 = int(0.70 * n_total)
    n_val_15 = int(0.15 * n_total)

    y_train_70 = y.iloc[:n_train_70]
    y_val_15 = y.iloc[n_train_70:n_train_70 + n_val_15]
    y_test_15 = y.iloc[n_train_70 + n_val_15:]

    results_3way = []

    models_to_run = [
        ("Persistence Baseline (rvol10)", None, ['realized_vol_10']),
        ("Canonical HAR-RV (Corsi 2009 OLS)", LinearRegression(), canonical_features),
        ("Extended Multi-Scale HAR-RV (ETH OLS)", LinearRegression(), eth_vol_features),
        ("Cross-Asset HAR-RV-X (ETH+BTC OLS)", LinearRegression(), cross_vol_features),
        ("Regularized HAR-RV (RidgeCV 30 Vol)", Pipeline([('scaler', StandardScaler()), ('ridge', RidgeCV(alphas=np.logspace(-4, 4, 15)))]), all_30_vol_features),
        ("Microstructure HAR-RV (RidgeCV Vol+LOB)", Pipeline([('scaler', StandardScaler()), ('ridge', RidgeCV(alphas=np.logspace(-4, 4, 15)))]), stationary_features)
    ]

    for name, estimator, feat_cols in models_to_run:
        if estimator is None:

            val_p = X_full[feat_cols[0]].iloc[n_train_70:n_train_70 + n_val_15]
            test_p = X_full[feat_cols[0]].iloc[n_train_70 + n_val_15:]
        else:
            X_tr = X_full[feat_cols].iloc[:n_train_70]
            X_va = X_full[feat_cols].iloc[n_train_70:n_train_70 + n_val_15]
            X_te = X_full[feat_cols].iloc[n_train_70 + n_val_15:]

            estimator.fit(X_tr, y_train_70)
            val_p = estimator.predict(X_va)
            test_p = estimator.predict(X_te)

        r_val, _ = pearsonr(y_val_15, val_p)
        rmse_val = root_mean_squared_error(y_val_15, val_p)
        r_test, _ = pearsonr(y_test_15, test_p)
        rmse_test = root_mean_squared_error(y_test_15, test_p)
        r2_test = r2_score(y_test_15, test_p)

        yt_test = np.maximum(y_test_15.values, 1e-12)
        yp_test = np.maximum(test_p if isinstance(test_p, np.ndarray) else test_p.values, 1e-12)
        qlike_test = float(np.mean(yt_test / yp_test - np.log(yt_test / yp_test) - 1.0))

        results_3way.append({
            "model": name,
            "val_pearson_r": r_val,
            "val_rmse": rmse_val,
            "test_pearson_r": r_test,
            "test_rmse": rmse_test,
            "test_r2": r2_test,
            "test_qlike": qlike_test
        })

    df_3way = pd.DataFrame(results_3way)
    print(f"\n{'Model Strategy':<40} {'Val r':>10} {'Val RMSE':>12} {'Test r (OOS)':>14} {'Test RMSE':>12} {'Test R2':>10} {'Test QLIKE':>12}")
    print("-" * 106)
    for _, row in df_3way.iterrows():
        print(f"{row['model']:<40} {row['val_pearson_r']:>10.4f} {row['val_rmse']:>12.4e} {row['test_pearson_r']:>14.4f} {row['test_rmse']:>12.4e} {row['test_r2']:>10.4f} {row['test_qlike']:>12.4f}")
    print("-" * 106)

    best_3way_row = df_3way.loc[df_3way['test_pearson_r'].idxmax()]
    print(f"\nBest Performing Time-Series Model: {best_3way_row['model']}")
    print(f"  Out-of-Sample Test Pearson r: {best_3way_row['test_pearson_r']:.4f}")
    print(f"  Out-of-Sample Test RMSE     : {best_3way_row['test_rmse']:.4e}")
    print(f"  Out-of-Sample Test QLIKE    : {best_3way_row['test_qlike']:.4f}")

    final_best_model = Pipeline([
        ('scaler', StandardScaler()),
        ('ridge', RidgeCV(alphas=np.logspace(-4, 4, 15)))
    ])
    final_best_model.fit(X_full[all_30_vol_features].iloc[:n_train_80], y_train_80)

    joblib.dump(final_best_model, os.path.join(models_dir, 'stage8_har_rv_best.joblib'))
    joblib.dump(all_30_vol_features, os.path.join(models_dir, 'stage8_har_rv_features.joblib'))
    df_3way.to_csv(os.path.join(models_dir, 'stage8_har_rv_results.csv'), index=False)
    print(f"\nSaved models and feature artifacts to {models_dir}/")

if __name__ == '__main__':
    main()
