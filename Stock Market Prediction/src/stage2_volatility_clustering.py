"""
Feature Engineering v3 — FINAL: Proven v2 cleaning + targeted feature additions + Optuna

Key lessons from experimentation:
  - fillna(0) on SOL NaN rows HURTS (injects noise → drops r from 0.64 to 0.53)
  - dropna() is correct: it preserves only clean, aligned data
  - v2's 90-feature set with dropna gives r=0.644 as the true baseline
  - Targeted additions (ret_squared, abs_return, rvol_10) + Optuna tuning can push further
"""

import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from scipy.stats import pearsonr
import xgboost as xgb
import lightgbm as lgb
import optuna
import os
import joblib
import warnings
warnings.filterwarnings('ignore')

from stage2_realized_volatility import load_and_merge_data, engineer_financial_features

def add_targeted_features(df):
    """Add only high-value features on top of v2."""
    for prefix in ['', '_BTC']:
        ret_col = f'log_return{prefix}'
        if ret_col not in df.columns:
            mid_col = f'mid_price{prefix}'
            df[ret_col] = np.log(df[mid_col] / df[mid_col].shift(1))

        df[f'ret_squared{prefix}'] = df[ret_col] ** 2

        df[f'abs_return{prefix}'] = df[ret_col].abs()

    for prefix in ['', '_BTC', '_SOL']:
        ret_col = f'log_return{prefix}'
        if ret_col not in df.columns:
            mid_col = f'mid_price{prefix}'
            df[ret_col] = np.log(df[mid_col] / df[mid_col].shift(1))
        col = f'realized_vol_10{prefix}'
        if col not in df.columns:
            df[col] = df[ret_col].rolling(10).std()
    return df

def clean_data_strict(df):
    """Use v2's proven strict cleaning: drop all NaN rows."""
    features = [c for c in df.columns if c not in ['timestamp', 'label']]
    X = df[features].astype(np.float32)
    y = df['label'].astype(np.float32)
    X = X.replace([np.inf, -np.inf], np.nan)
    y = y.replace([np.inf, -np.inf], np.nan)
    mask = X.notna().all(axis=1) & y.notna()
    X = X[mask]
    y = y[mask]
    print(f"Kept {len(X)} clean rows (dropped {(~mask).sum()} NaN/Inf rows).")
    return X, y

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'datasets', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')

    df = load_and_merge_data(
        os.path.join(train_dir, 'ETH.csv'),
        os.path.join(train_dir, 'BTC.csv'),
        os.path.join(train_dir, 'SOL.csv')
    )
    df = engineer_financial_features(df)
    df = add_targeted_features(df)
    X, y = clean_data_strict(df)

    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)
    print(f"Train: {len(X_train)} | Val: {len(X_val)} | Features: {X.shape[1]}")

    print("\n--- Step 1: Default LightGBM on v3 features ---")
    base = lgb.LGBMRegressor(
        n_estimators=1000, learning_rate=0.05, max_depth=6,
        num_leaves=31, random_state=42, n_jobs=-1
    )
    base.fit(X_train, y_train, eval_set=[(X_val, y_val)],
             callbacks=[lgb.early_stopping(50, verbose=False)])
    base_preds = base.predict(X_val)
    base_corr, _ = pearsonr(y_val, base_preds)
    print(f"Default LightGBM → r = {base_corr:.6f} (stopped @ iter {base.best_iteration_})")

    print("\n--- Step 2: Optuna Tuning (60 trials, optimizing Pearson r) ---")
    X_tune = X_train.iloc[-50000:]
    y_tune = y_train.iloc[-50000:]

    def objective(trial):
        params = {
            'n_estimators': 3000,
            'learning_rate': trial.suggest_float('lr', 0.005, 0.15, log=True),
            'max_depth': trial.suggest_int('max_depth', 3, 8),
            'num_leaves': trial.suggest_int('num_leaves', 15, 127),
            'min_child_samples': trial.suggest_int('min_child_samples', 5, 100),
            'subsample': trial.suggest_float('subsample', 0.5, 1.0),
            'colsample_bytree': trial.suggest_float('colsample_bytree', 0.4, 1.0),
            'reg_alpha': trial.suggest_float('reg_alpha', 1e-8, 10.0, log=True),
            'reg_lambda': trial.suggest_float('reg_lambda', 1e-8, 10.0, log=True),
            'random_state': 42,
            'n_jobs': -1,
        }
        model = lgb.LGBMRegressor(**params)
        model.fit(
            X_tune, y_tune,
            eval_set=[(X_val, y_val)],
            callbacks=[lgb.early_stopping(30, verbose=False)]
        )
        preds = model.predict(X_val)
        corr, _ = pearsonr(y_val, preds)
        return corr

    optuna.logging.set_verbosity(optuna.logging.WARNING)
    study = optuna.create_study(direction='maximize')
    study.optimize(objective, n_trials=60)

    print(f"\nBest Optuna trial → r = {study.best_value:.6f}")
    print(f"Best params: {study.best_params}")

    print("\n--- Step 3: Final LightGBM on full training data ---")
    best = study.best_params
    final_lgb = lgb.LGBMRegressor(
        n_estimators=5000,
        learning_rate=best['lr'],
        max_depth=best['max_depth'],
        num_leaves=best['num_leaves'],
        min_child_samples=best['min_child_samples'],
        subsample=best['subsample'],
        colsample_bytree=best['colsample_bytree'],
        reg_alpha=best['reg_alpha'],
        reg_lambda=best['reg_lambda'],
        random_state=42, n_jobs=-1
    )
    final_lgb.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        callbacks=[lgb.early_stopping(50, verbose=False)]
    )
    lgb_preds = final_lgb.predict(X_val)
    lgb_corr, _ = pearsonr(y_val, lgb_preds)

    print("\n--- Step 4: XGBoost GPU ---")
    xgb_model = xgb.XGBRegressor(
        n_estimators=5000,
        learning_rate=best['lr'],
        max_depth=best['max_depth'],
        subsample=best['subsample'],
        colsample_bytree=best['colsample_bytree'],
        reg_alpha=best['reg_alpha'],
        reg_lambda=best['reg_lambda'],
        tree_method='hist', device='cuda',
        early_stopping_rounds=50, random_state=42
    )
    xgb_model.fit(X_train, y_train, eval_set=[(X_val, y_val)], verbose=False)
    xgb_preds = xgb_model.predict(X_val)
    xgb_corr, _ = pearsonr(y_val, xgb_preds)

    print("\n" + "="*65)
    print("              FINAL RESULTS")
    print("="*65)
    print(f"  Baseline (raw features)          →  r = 0.0947")
    print(f"  v2 default (90 feats)            →  r = 0.6441")
    print(f"  v3 default ({X.shape[1]} feats)            →  r = {base_corr:.4f}")
    print(f"  v3 Optuna LightGBM               →  r = {lgb_corr:.4f} (iter {final_lgb.best_iteration_})")
    print(f"  v3 XGBoost GPU                   →  r = {xgb_corr:.4f} (iter {xgb_model.best_iteration})")
    print("="*65)

    models_dir = os.path.join(script_dir, '..', 'models')
    os.makedirs(models_dir, exist_ok=True)
    best_corr = max(lgb_corr, xgb_corr)
    if lgb_corr >= xgb_corr:
        joblib.dump(final_lgb, os.path.join(models_dir, 'v3_champion.joblib'))
        print(f"\n🏆 Champion: LightGBM (r={lgb_corr:.6f})")
    else:
        joblib.dump(xgb_model, os.path.join(models_dir, 'v3_champion.joblib'))
        print(f"\n🏆 Champion: XGBoost (r={xgb_corr:.6f})")

    imp = pd.DataFrame({'feature': X.columns, 'importance': final_lgb.feature_importances_})
    imp = imp.sort_values('importance', ascending=False)
    print("\nTop 15 Features:")
    print(imp.head(15).to_string(index=False))

if __name__ == "__main__":
    main()
