"""
Rigorous 3-Way Chronological Benchmark (Train / Validation / Test)
==================================================================
Eliminates selection bias and validation leakage by implementing a strict
3-way chronological split:
  - 70% Train      (~424,000 rows): Model fitting
  - 15% Validation (~90,900 rows) : Early stopping & SLSQP blend weight optimization
  - 15% Test       (~90,900 rows) : Untouched holdout set scored ONCE for true OOS metrics

Baselines Included:
  1. Naive Volatility Persistence: y_hat = realized_vol_10
  2. Univariate OLS: y_hat = alpha + beta * realized_vol_10
  3. Calibrated Multivariate RidgeCV (StandardScaler + RidgeCV on 30 Vol Features)
  4. LightGBM Conservative
  5. XGBoost Conservative
  6. Equal-Weight Tree Blend (50% LGBM + 50% XGB)
  7. Stage 5 SLSQP Ensemble (weights learned on Val, scored on Test)
  8. Stage 7 Optuna-Tuned Models & Blend
  9. Econometric-Tree Hybrid Blend (1/3 RidgeCV + 1/3 LGBM + 1/3 XGB)
"""

import os
import joblib
import warnings
import argparse
import datetime
import numpy as np
import pandas as pd
from scipy.stats import pearsonr
from scipy.optimize import minimize
from sklearn.metrics import root_mean_squared_error
from sklearn.linear_model import LinearRegression, RidgeCV
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
import lightgbm as lgb
import xgboost as xgb

import sys
sys.path.append(os.path.dirname(os.path.abspath(__file__)))
from stage4_multiscale_pipeline import load_and_merge, engineer_multiscale_features, clean_and_prepare

warnings.filterwarnings('ignore', category=FutureWarning)
warnings.filterwarnings('ignore', category=UserWarning)

def main():
    parser = argparse.ArgumentParser(description="Rigorous 3-Way Chronological Benchmark")
    parser.add_argument('--export-app', action='store_true', default=False,
                        help="Export models and cached predictions for Streamlit terminal app")
    args = parser.parse_args()

    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    models_dir = os.path.join(script_dir, '..', 'models')
    os.makedirs(models_dir, exist_ok=True)

    print("=" * 80)
    print(" Rigorous 3-Way Chronological Benchmark (Train / Val / Held-Out Test)")
    print("=" * 80)

    df = load_and_merge(train_dir)
    df = engineer_multiscale_features(df)
    X, y = clean_and_prepare(df)

    n_total = len(X)
    n_train = int(0.70 * n_total)
    n_val = int(0.15 * n_total)
    n_test = n_total - n_train - n_val

    X_train, y_train = X.iloc[:n_train], y.iloc[:n_train]
    X_val, y_val = X.iloc[n_train:n_train + n_val], y.iloc[n_train:n_train + n_val]
    X_test, y_test = X.iloc[n_train + n_val:], y.iloc[n_train + n_val:]

    print(f"\nSplit Distribution:")
    print(f"  - Train Set      : {len(X_train):>7,} rows ({len(X_train)/n_total*100:.1f}%) [Model Fitting]")
    print(f"  - Validation Set : {len(X_val):>7,} rows ({len(X_val)/n_total*100:.1f}%) [Early Stopping & Blend Weights]")
    print(f"  - Test Set (OOS) : {len(X_test):>7,} rows ({len(X_test)/n_total*100:.1f}%) [Strictly Held-Out Final Evaluation]")
    print(f"  - Features       : {X.shape[1]} columns\n")

    results_val = {}
    results_test = {}

    def record_metrics(name, preds_val, preds_test):
        r_val, _ = pearsonr(y_val, preds_val)
        rmse_val = root_mean_squared_error(y_val, preds_val)
        results_val[name] = (r_val, rmse_val)

        r_test, _ = pearsonr(y_test, preds_test)
        rmse_test = root_mean_squared_error(y_test, preds_test)
        results_test[name] = (r_test, rmse_test)
        return r_test, rmse_test

    p_val_naive = X_val['realized_vol_10'].values
    p_test_naive = X_test['realized_vol_10'].values
    record_metrics("Baseline 1: Persistence (rvol10)", p_val_naive, p_test_naive)

    ols = LinearRegression()
    ols.fit(X_train[['realized_vol_10']], y_train)
    p_val_ols = ols.predict(X_val[['realized_vol_10']])
    p_test_ols = ols.predict(X_test[['realized_vol_10']])
    record_metrics("Baseline 2: Univariate OLS (rvol10)", p_val_ols, p_test_ols)

    vol_cols = [c for c in X.columns if any(k in c for k in ['realized_vol', 'ewma_vol', 'ret_squared', 'abs_return'])]
    ridge_pipe = Pipeline([
        ('scaler', StandardScaler()),
        ('ridge', RidgeCV(alphas=np.logspace(-4, 4, 15)))
    ])
    ridge_pipe.fit(X_train[vol_cols], y_train)
    p_val_ridge = ridge_pipe.predict(X_val[vol_cols])
    p_test_ridge = ridge_pipe.predict(X_test[vol_cols])
    record_metrics("Baseline 3: Calibrated RidgeCV (30 Vol)", p_val_ridge, p_test_ridge)

    print("Training LightGBM Conservative on Train split...")
    lgbm_cons = lgb.LGBMRegressor(
        n_estimators=3000, max_depth=4, num_leaves=15,
        learning_rate=0.05, colsample_bytree=0.70, subsample=0.80,
        random_state=42, n_jobs=-1, verbose=-1
    )
    lgbm_cons.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        callbacks=[lgb.early_stopping(50, verbose=False)]
    )
    p_val_lgbm_c = lgbm_cons.predict(X_val)
    p_test_lgbm_c = lgbm_cons.predict(X_test)
    record_metrics("LightGBM Conservative (Depth 4)", p_val_lgbm_c, p_test_lgbm_c)

    print("Training XGBoost Conservative on Train split (CUDA)...")
    xgb_cons = xgb.XGBRegressor(
        n_estimators=3000, max_depth=4, learning_rate=0.05,
        colsample_bytree=0.60, subsample=0.80, tree_method='hist',
        device='cuda', early_stopping_rounds=50, random_state=42, verbosity=0
    )
    xgb_cons.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        verbose=False
    )
    xgb_cons.set_params(device='cpu')
    p_val_xgb_c = xgb_cons.predict(X_val)
    p_test_xgb_c = xgb_cons.predict(X_test)
    record_metrics("XGBoost Conservative (Depth 4)", p_val_xgb_c, p_test_xgb_c)

    print("Training LightGBM Deep (Depth 6)...")
    lgbm_deep = lgb.LGBMRegressor(
        n_estimators=3000, max_depth=6, num_leaves=40,
        learning_rate=0.03, colsample_bytree=0.80, subsample=0.80,
        random_state=42, n_jobs=-1, verbose=-1
    )
    lgbm_deep.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        callbacks=[lgb.early_stopping(50, verbose=False)]
    )
    p_val_lgbm_d = lgbm_deep.predict(X_val)
    p_test_lgbm_d = lgbm_deep.predict(X_test)
    record_metrics("LightGBM Deep (Depth 6)", p_val_lgbm_d, p_test_lgbm_d)

    print("Training XGBoost Deep (Depth 6)...")
    xgb_deep = xgb.XGBRegressor(
        n_estimators=3000, max_depth=6, learning_rate=0.03,
        colsample_bytree=0.80, subsample=0.80, tree_method='hist',
        device='cuda', early_stopping_rounds=50, random_state=42, verbosity=0
    )
    xgb_deep.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        verbose=False
    )
    xgb_deep.set_params(device='cpu')
    p_val_xgb_d = xgb_deep.predict(X_val)
    p_test_xgb_d = xgb_deep.predict(X_test)
    record_metrics("XGBoost Deep (Depth 6)", p_val_xgb_d, p_test_xgb_d)

    p_val_equal = 0.5 * p_val_lgbm_c + 0.5 * p_val_xgb_c
    p_test_equal = 0.5 * p_test_lgbm_c + 0.5 * p_test_xgb_c
    record_metrics("Equal-Weight Tree Blend (50/50)", p_val_equal, p_test_equal)

    base_val_matrix = np.column_stack([p_val_lgbm_c, p_val_xgb_c, p_val_lgbm_d, p_val_xgb_d])
    base_test_matrix = np.column_stack([p_test_lgbm_c, p_test_xgb_c, p_test_lgbm_d, p_test_xgb_d])
    base_names = ['LGBM_Cons', 'XGB_Cons', 'LGBM_Deep', 'XGB_Deep']

    def slsqp_blend(pred_matrix, y_target):
        def obj(w):
            norm_w = w / np.sum(w)
            return -pearsonr(y_target, pred_matrix @ norm_w)[0]
        w0 = np.ones(pred_matrix.shape[1]) / pred_matrix.shape[1]
        bounds = [(0.0, 1.0)] * pred_matrix.shape[1]
        res = minimize(obj, w0, bounds=bounds, method='SLSQP')
        return res.x / np.sum(res.x)

    stage5_weights = slsqp_blend(base_val_matrix, y_val)
    p_val_stage5 = base_val_matrix @ stage5_weights
    p_test_stage5 = base_test_matrix @ stage5_weights
    record_metrics("Stage 5: SLSQP Tree Blend (Tuned on Val)", p_val_stage5, p_test_stage5)

    optuna_lgb_params = {
        'n_estimators': 1200, 'max_depth': 5, 'num_leaves': 37,
        'learning_rate': 0.015, 'colsample_bytree': 0.50, 'subsample': 0.60,
        'min_child_samples': 50, 'random_state': 42, 'n_jobs': -1, 'verbose': -1
    }
    optuna_xgb_params = {
        'n_estimators': 2000, 'max_depth': 4, 'learning_rate': 0.035,
        'colsample_bytree': 0.50, 'subsample': 0.75, 'min_child_weight': 7,
        'tree_method': 'hist', 'device': 'cuda', 'early_stopping_rounds': 50,
        'random_state': 42, 'verbosity': 0
    }

    print("\nTraining Stage 7 Tuned Models on Train split...")
    lgbm_tuned = lgb.LGBMRegressor(**optuna_lgb_params)
    lgbm_tuned.fit(X_train, y_train, eval_set=[(X_val, y_val)], callbacks=[lgb.early_stopping(50, verbose=False)])
    p_val_lgbm_t = lgbm_tuned.predict(X_val)
    p_test_lgbm_t = lgbm_tuned.predict(X_test)
    record_metrics("LightGBM Tuned (Stage 7)", p_val_lgbm_t, p_test_lgbm_t)

    xgb_tuned = xgb.XGBRegressor(**optuna_xgb_params)
    xgb_tuned.fit(X_train, y_train, eval_set=[(X_val, y_val)], verbose=False)
    xgb_tuned.set_params(device='cpu')
    p_val_xgb_t = xgb_tuned.predict(X_val)
    p_test_xgb_t = xgb_tuned.predict(X_test)
    record_metrics("XGBoost Tuned (Stage 7)", p_val_xgb_t, p_test_xgb_t)

    p_val_hybrid = (p_val_ridge + p_val_lgbm_c + p_val_xgb_c) / 3.0
    p_test_hybrid = (p_test_ridge + p_test_lgbm_c + p_test_xgb_c) / 3.0
    record_metrics("Econometric-Tree Hybrid (1/3 Ridge, 1/3 LGB, 1/3 XGB)", p_val_hybrid, p_test_hybrid)

    print("\n" + "=" * 94)
    print("                        RIGOROUS 3-WAY BENCHMARK SUMMARY")
    print("=" * 94)
    print(f"{'Strategy / Model':<44} | {'Val r':<9} | {'Val RMSE':<12} | {'Test r (OOS)':<12} | {'Test RMSE':<12}")
    print("-" * 94)
    for name in results_test.keys():
        vr, vrmse = results_val[name]
        tr, trmse = results_test[name]
        print(f"{name:<44} | {vr:<9.4f} | {vrmse:<12.8f} | {tr:<12.4f} | {trmse:<12.8f}")
    print("=" * 94)

    summary_df = pd.DataFrame([
        {
            'model': name,
            'val_pearson_r': results_val[name][0],
            'val_rmse': results_val[name][1],
            'test_pearson_r': results_test[name][0],
            'test_rmse': results_test[name][1]
        }
        for name in results_test.keys()
    ])
    out_csv = os.path.join(models_dir, 'rigorous_benchmark_3way_results.csv')
    summary_df.to_csv(out_csv, index=False)
    print(f"\nBenchmark results saved cleanly to: {out_csv}")

    if args.export_app:
        print("\n" + "=" * 80)
        print(" Exporting Artifacts for Streamlit Quantitative Terminal App")
        print("=" * 80)

        xgb_cons.set_params(device='cpu')

        ridge_path = os.path.join(models_dir, 'final_hybrid_ridge.joblib')
        lgbm_path = os.path.join(models_dir, 'final_hybrid_lgbm.joblib')
        xgb_path = os.path.join(models_dir, 'final_hybrid_xgb.joblib')
        feat_path = os.path.join(models_dir, 'final_feature_list.joblib')
        vol_path = os.path.join(models_dir, 'final_vol_cols.joblib')

        joblib.dump(ridge_pipe, ridge_path)
        joblib.dump(lgbm_cons, lgbm_path)
        joblib.dump(xgb_cons, xgb_path)
        joblib.dump(list(X.columns), feat_path)
        joblib.dump(vol_cols, vol_path)

        processed_dir = os.path.join(script_dir, '..', 'data', 'processed')
        os.makedirs(processed_dir, exist_ok=True)
        cache_path = os.path.join(processed_dir, 'app_cache.joblib')

        q33 = float(np.percentile(y_train, 33))
        q67 = float(np.percentile(y_train, 67))

        meta = {
            'n_train': n_train,
            'n_val': n_val,
            'n_test': n_test,
            'horizon_text': "10-second",
            'q33': q33,
            'q67': q67,
            'created_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
            'feature_count': int(X.shape[1]),
        }

        def build_slice_df(X_slice, y_slice, p_naive, p_ridge, p_lgbm, p_xgb, p_blend, p_hyb):
            data_dict = {
                'row_id': X_slice.index.values,
                'timestamp': df['timestamp'].loc[X_slice.index].values,
                'label': y_slice.values.astype(np.float32)
            }
            for c in X_slice.columns:
                data_dict[c] = X_slice[c].values.astype(np.float32)
            data_dict['pred_persistence'] = p_naive.astype(np.float32)
            data_dict['pred_ridge'] = p_ridge.astype(np.float32)
            data_dict['pred_lgbm'] = p_lgbm.astype(np.float32)
            data_dict['pred_xgb'] = p_xgb.astype(np.float32)
            data_dict['pred_tree_blend'] = p_blend.astype(np.float32)
            data_dict['pred_hybrid'] = p_hyb.astype(np.float32)
            return pd.DataFrame(data_dict)

        val_df = build_slice_df(X_val, y_val, p_val_naive, p_val_ridge, p_val_lgbm_c, p_val_xgb_c, p_val_equal, p_val_hybrid)
        test_df = build_slice_df(X_test, y_test, p_test_naive, p_test_ridge, p_test_lgbm_c, p_test_xgb_c, p_test_equal, p_test_hybrid)

        pred_cols = ['pred_persistence', 'pred_ridge', 'pred_lgbm', 'pred_xgb', 'pred_tree_blend', 'pred_hybrid']
        metrics = {'val': {}, 'test': {}}
        for col in pred_cols:
            r_v, _ = pearsonr(y_val, val_df[col].values)
            rmse_v = root_mean_squared_error(y_val, val_df[col].values)
            metrics['val'][col] = {'r': float(r_v), 'rmse': float(rmse_v)}

            r_t, _ = pearsonr(y_test, test_df[col].values)
            rmse_t = root_mean_squared_error(y_test, test_df[col].values)
            metrics['test'][col] = {'r': float(r_t), 'rmse': float(rmse_t)}

        cache_data = {
            'meta': meta,
            'val': val_df,
            'test': test_df,
            'metrics': metrics
        }

        joblib.dump(cache_data, cache_path, compress=3)

        exported_files = [ridge_path, lgbm_path, xgb_path, feat_path, vol_path, cache_path]
        print("\nExported Artifacts:")
        for fp in exported_files:
            size_mb = os.path.getsize(fp) / (1024 * 1024)
            print(f"  - {os.path.basename(fp):<30}: {size_mb:>7.2f} MB ({fp})")

        print("\nTest Slice Metrics (OOS):")
        for col in pred_cols:
            print(f"  - {col:<18}: r = {metrics['test'][col]['r']:.4f}, RMSE = {metrics['test'][col]['rmse']:.8f}")

if __name__ == '__main__':
    main()
