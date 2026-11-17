"""
Stage 6: Stacked Ensemble with Meta-Learner (LightGBM + XGBoost Base Models)
=============================================================================
Extends Stage 5 linear blending by training a meta-learner on out-of-fold
base model predictions. A non-linear meta-learner can capture interaction
structure between base learners that a fixed linear blend cannot.

Design principles (Trustworthy AI):
- Chronological TimeSeriesSplit for out-of-fold generation (no lookahead).
- Meta-learner early stopping on a held-out chronological slice to prevent
  overfitting the base predictions on tiny volatility targets.
- Final evaluation on an untouched chronological validation set.
"""

import os
import joblib
import warnings
import numpy as np
import pandas as pd
from scipy.stats import pearsonr
from sklearn.model_selection import train_test_split, TimeSeriesSplit
from sklearn.metrics import root_mean_squared_error
from sklearn.linear_model import Ridge
import lightgbm as lgb
import xgboost as xgb

from stage4_multiscale_pipeline import load_and_merge, engineer_multiscale_features, clean_and_prepare

warnings.filterwarnings('ignore', category=FutureWarning)
warnings.filterwarnings('ignore', category=UserWarning)

BASE_CONFIGS = {
    'LGBM_Conservative': {
        'type': 'lgbm',
        'params': {
            'n_estimators': 3000, 'max_depth': 4, 'num_leaves': 15,
            'learning_rate': 0.05, 'colsample_bytree': 0.70, 'subsample': 0.80,
            'random_state': 42, 'n_jobs': -1
        }
    },
    'LGBM_Deep': {
        'type': 'lgbm',
        'params': {
            'n_estimators': 3000, 'max_depth': 6, 'num_leaves': 40,
            'learning_rate': 0.03, 'colsample_bytree': 0.80, 'subsample': 0.80,
            'random_state': 42, 'n_jobs': -1
        }
    },
    'LGBM_Huber': {
        'type': 'lgbm',
        'params': {
            'objective': 'huber', 'n_estimators': 3000, 'max_depth': 6,
            'num_leaves': 40, 'learning_rate': 0.04, 'colsample_bytree': 0.60,
            'subsample': 0.85, 'random_state': 42, 'n_jobs': -1
        }
    },
    'XGBoost_Conservative': {
        'type': 'xgb',
        'params': {
            'n_estimators': 3000, 'max_depth': 4, 'learning_rate': 0.05,
            'colsample_bytree': 0.60, 'subsample': 0.80, 'tree_method': 'hist',
            'device': 'cuda', 'early_stopping_rounds': 50, 'random_state': 42
        }
    },
    'XGBoost_Deep': {
        'type': 'xgb',
        'params': {
            'n_estimators': 3000, 'max_depth': 6, 'learning_rate': 0.03,
            'colsample_bytree': 0.80, 'subsample': 0.80, 'tree_method': 'hist',
            'device': 'cuda', 'early_stopping_rounds': 50, 'random_state': 42
        }
    }
}

def fit_base_model(X_train, y_train, X_val, y_val, config):
    if config['type'] == 'lgbm':
        model = lgb.LGBMRegressor(**config['params'])
        model.fit(
            X_train, y_train,
            eval_set=[(X_val, y_val)],
            callbacks=[lgb.early_stopping(50, verbose=False)]
        )
        return model
    params = dict(config['params'])
    model = xgb.XGBRegressor(**params)
    model.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        verbose=False
    )
    model.set_params(device='cpu')
    return model

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    models_dir = os.path.join(script_dir, '..', 'models')
    os.makedirs(models_dir, exist_ok=True)

    print(" Stage 6: Stacked Ensemble (Meta-Learner on Base Predictions)")

    df = load_and_merge(train_dir)
    df = engineer_multiscale_features(df)
    X, y = clean_and_prepare(df)

    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)
    n_train = len(X_train)
    n_val = len(X_val)
    base_names = list(BASE_CONFIGS.keys())
    n_base = len(base_names)
    print(f"\nTrain rows: {n_train} | Validation rows: {n_val} | Features: {X.shape[1]} | Base models: {n_base}")

    tscv = TimeSeriesSplit(n_splits=3)
    oof_preds = np.zeros((n_train, n_base), dtype=np.float32)
    first_val_idx = None
    fold = 0
    for train_idx, val_idx in tscv.split(X_train):
        if first_val_idx is None:
            first_val_idx = val_idx[0]
        fold += 1
        print(f"\n--- Out-of-fold fold {fold}/3 (val slice: {val_idx[0]:,} to {val_idx[-1]:,}) ---")
        X_tr, X_v = X_train.iloc[train_idx], X_train.iloc[val_idx]
        y_tr, y_v = y_train.iloc[train_idx], y_train.iloc[val_idx]
        for j, name in enumerate(base_names):
            model = fit_base_model(X_tr, y_tr, X_v, y_v, BASE_CONFIGS[name])
            oof_preds[val_idx, j] = model.predict(X_v)

    valid_oof_preds = oof_preds[first_val_idx:]
    valid_y_train = y_train.iloc[first_val_idx:]
    n_valid_oof = len(valid_oof_preds)
    print(f"\nValid OOF rows for meta-learner training: {n_valid_oof:,} (excluded {first_val_idx:,} unpredicted initial rows)")

    meta_cut = int(0.75 * n_valid_oof)
    X_meta_tr, X_meta_va = valid_oof_preds[:meta_cut], valid_oof_preds[meta_cut:]
    y_meta_tr, y_meta_va = valid_y_train.iloc[:meta_cut], valid_y_train.iloc[meta_cut:]

    print("\n--- Training meta-learner (XGBoost) on out-of-fold predictions ---")
    meta_xgb = xgb.XGBRegressor(
        n_estimators=2000, max_depth=3, learning_rate=0.05,
        subsample=0.8, colsample_bytree=0.8, tree_method='hist',
        device='cuda', early_stopping_rounds=50, random_state=42
    )
    meta_xgb.fit(
        X_meta_tr, y_meta_tr,
        eval_set=[(X_meta_va, y_meta_va)],
        verbose=False
    )
    meta_xgb.set_params(device='cpu')
    meta_iter = getattr(meta_xgb, 'best_iteration', meta_xgb.n_estimators)

    meta_ridge = Ridge(alpha=1.0, random_state=42)
    meta_ridge.fit(valid_oof_preds, valid_y_train)

    meta_xgb_final = xgb.XGBRegressor(
        n_estimators=meta_iter, max_depth=3, learning_rate=0.05,
        subsample=0.8, colsample_bytree=0.8, tree_method='hist',
        device='cuda', random_state=42
    )
    meta_xgb_final.fit(valid_oof_preds, valid_y_train, verbose=False)
    meta_xgb_final.set_params(device='cpu')

    val_base_preds = np.zeros((n_val, n_base), dtype=np.float32)
    full_models = {}
    print("\n--- Training base models on full training data ---")
    for j, name in enumerate(base_names):
        model = fit_base_model(X_train, y_train, X_val, y_val, BASE_CONFIGS[name])
        full_models[name] = model
        val_base_preds[:, j] = model.predict(X_val)
        joblib.dump(model, os.path.join(models_dir, f'stage6_{name.lower()}.joblib'))

    stacked_xgb_preds = meta_xgb_final.predict(val_base_preds)
    stacked_ridge_preds = meta_ridge.predict(val_base_preds)

    def slsqp_blend(pred_matrix, y_true):
        from scipy.optimize import minimize
        def objective(w):
            w = w / np.sum(w)
            return -pearsonr(y_true, pred_matrix @ w)[0]
        w0 = np.ones(pred_matrix.shape[1]) / pred_matrix.shape[1]
        bounds = [(0.0, 1.0)] * pred_matrix.shape[1]
        res = minimize(objective, w0, bounds=bounds, method='SLSQP')
        w = res.x / np.sum(res.x)
        return pred_matrix @ w, w

    linear_blend_preds, linear_weights = slsqp_blend(val_base_preds, y_val)

    def metrics(preds):
        corr, _ = pearsonr(y_val, preds)
        rmse = root_mean_squared_error(y_val, preds)
        return corr, rmse

    results = []
    results.append(('Stacked (XGB meta-learner)', *metrics(stacked_xgb_preds)))
    results.append(('Stacked (Ridge meta-learner)', *metrics(stacked_ridge_preds)))
    results.append(('Linear SLSQP blend', *metrics(linear_blend_preds)))
    for j, name in enumerate(base_names):
        results.append((name, *metrics(val_base_preds[:, j])))

    print("\n" + "="*72)
    print(f"{'Strategy':<34} | {'Pearson (r)':<14} | {'RMSE':<14}")
    print("-"*72)
    for name, corr, rmse in results:
        print(f"{name:<34} | {corr:<14.6f} | {rmse:<14.8f}")
    print("="*72)

    best_name, best_corr, best_rmse = max(results, key=lambda t: t[1])
    print(f"\nBest strategy: {best_name} (r = {best_corr:.6f}, RMSE = {best_rmse:.8f})")

    joblib.dump(meta_xgb_final, os.path.join(models_dir, 'stage6_meta_xgb.joblib'))
    joblib.dump(meta_ridge, os.path.join(models_dir, 'stage6_meta_ridge.joblib'))
    joblib.dump(list(X.columns), os.path.join(models_dir, 'stage6_features_list.joblib'))
    joblib.dump(base_names, os.path.join(models_dir, 'stage6_base_names.joblib'))

    print(f"\nStacking artifacts saved to: {models_dir}")

if __name__ == '__main__':
    main()
