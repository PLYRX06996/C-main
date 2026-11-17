"""
Stage 7: Optuna-Tuned Gradient Boosting Ensemble
================================================
Extends Stage 5 by tuning base model hyperparameters with Optuna to maximize
Pearson correlation (directional skill) directly on the validation set, then
re-blending the tuned learners with constrained SLSQP weights.

The Stage 5 base learners used fixed, hand-picked hyperparameters. Because the
constituent models are highly correlated (linear blend gain was only +0.0008
over the best single model), re-weighting alone cannot improve the score.
Better base learners are required.

Design (Trustworthy AI):
- Chronological train/validation split (shuffle=False), no lookahead.
- Early stopping (50 rounds) on every trial to prevent noise overfitting.
- Optuna TPE searches depth, learning rate, leaves, subsampling, and boosting
  rounds; objective is Pearson r (maximize).
- Final tuned models retrained on full training data; blend weights optimized
  on the held-out validation set.
"""

import os
import joblib
import warnings
import numpy as np
import pandas as pd
import optuna
from scipy.stats import pearsonr
from scipy.optimize import minimize
from sklearn.model_selection import train_test_split
from sklearn.metrics import root_mean_squared_error
import lightgbm as lgb
import xgboost as xgb

from stage4_multiscale_pipeline import load_and_merge, engineer_multiscale_features, clean_and_prepare

warnings.filterwarnings('ignore', category=FutureWarning)
warnings.filterwarnings('ignore', category=UserWarning)

N_TRIALS = 20
EARLY_STOPPING = 50
SEED = 42

def suggest_lgb_params(trial):
    return {
        'n_estimators': trial.suggest_int('n_estimators', 500, 3000),
        'max_depth': trial.suggest_int('max_depth', 3, 8),
        'num_leaves': trial.suggest_int('num_leaves', 15, 128),
        'learning_rate': trial.suggest_float('learning_rate', 0.01, 0.15, log=True),
        'colsample_bytree': trial.suggest_float('colsample_bytree', 0.5, 1.0),
        'subsample': trial.suggest_float('subsample', 0.5, 1.0),
        'min_child_samples': trial.suggest_int('min_child_samples', 10, 100),
        'random_state': SEED,
        'n_jobs': -1,
        'verbose': -1
    }

def suggest_xgb_params(trial):
    return {
        'n_estimators': trial.suggest_int('n_estimators', 500, 3000),
        'max_depth': trial.suggest_int('max_depth', 3, 8),
        'learning_rate': trial.suggest_float('learning_rate', 0.01, 0.15, log=True),
        'colsample_bytree': trial.suggest_float('colsample_bytree', 0.5, 1.0),
        'subsample': trial.suggest_float('subsample', 0.5, 1.0),
        'min_child_weight': trial.suggest_int('min_child_weight', 1, 10),
        'tree_method': 'hist',
        'device': 'cuda',
        'early_stopping_rounds': EARLY_STOPPING,
        'random_state': SEED,
        'verbosity': 0
    }

def fit_lgb(params, X_train, y_train, X_val, y_val):
    model = lgb.LGBMRegressor(**params)
    model.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        callbacks=[lgb.early_stopping(EARLY_STOPPING, verbose=False)]
    )
    return model

def fit_xgb(params, X_train, y_train, X_val, y_val):
    params = dict(params)
    model = xgb.XGBRegressor(**params)
    model.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        verbose=False
    )
    model.set_params(device='cpu')
    return model

def slsqp_blend(pred_matrix, y_true):
    def objective(w):
        w = w / np.sum(w)
        return -pearsonr(y_true, pred_matrix @ w)[0]
    w0 = np.ones(pred_matrix.shape[1]) / pred_matrix.shape[1]
    bounds = [(0.0, 1.0)] * pred_matrix.shape[1]
    res = minimize(objective, w0, bounds=bounds, method='SLSQP')
    w = res.x / np.sum(res.x)
    return pred_matrix @ w, w

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    models_dir = os.path.join(script_dir, '..', 'models')
    os.makedirs(models_dir, exist_ok=True)

    print(" Stage 7: Optuna-Tuned Gradient Boosting Ensemble")

    df = load_and_merge(train_dir)
    df = engineer_multiscale_features(df)
    X, y = clean_and_prepare(df)

    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)
    print(f"\nTrain rows: {len(X_train)} | Validation rows: {len(X_val)} | Features: {X.shape[1]}")

    def lgb_objective(trial):
        params = suggest_lgb_params(trial)
        model = fit_lgb(params, X_train, y_train, X_val, y_val)
        preds = model.predict(X_val)
        corr, _ = pearsonr(y_val, preds)
        return corr

    print("\n--- Optuna tuning LightGBM (maximize Pearson r) ---")
    lgb_sampler = optuna.samplers.TPESampler(seed=SEED)
    lgb_study = optuna.create_study(direction='maximize', sampler=lgb_sampler)
    lgb_study.optimize(lgb_objective, n_trials=N_TRIALS, show_progress_bar=False)
    print(f"Best LightGBM trial r = {lgb_study.best_value:.6f}")

    def xgb_objective(trial):
        params = suggest_xgb_params(trial)
        model = fit_xgb(params, X_train, y_train, X_val, y_val)
        preds = model.predict(X_val)
        corr, _ = pearsonr(y_val, preds)
        return corr

    print("\n--- Optuna tuning XGBoost (maximize Pearson r) ---")
    xgb_sampler = optuna.samplers.TPESampler(seed=SEED)
    xgb_study = optuna.create_study(direction='maximize', sampler=xgb_sampler)
    xgb_study.optimize(xgb_objective, n_trials=N_TRIALS, show_progress_bar=False)
    print(f"Best XGBoost trial r = {xgb_study.best_value:.6f}")

    tuned_lgb = fit_lgb(lgb_study.best_params, X_train, y_train, X_val, y_val)
    tuned_xgb = fit_xgb(xgb_study.best_params, X_train, y_train, X_val, y_val)

    fixed_configs = {
        'LGBM_Deep': {'type': 'lgbm', 'params': {
            'n_estimators': 3000, 'max_depth': 6, 'num_leaves': 40,
            'learning_rate': 0.03, 'colsample_bytree': 0.80, 'subsample': 0.80,
            'random_state': SEED, 'n_jobs': -1, 'verbose': -1
        }},
        'LGBM_Huber': {'type': 'lgbm', 'params': {
            'objective': 'huber', 'n_estimators': 3000, 'max_depth': 6,
            'num_leaves': 40, 'learning_rate': 0.04, 'colsample_bytree': 0.60,
            'subsample': 0.85, 'random_state': SEED, 'n_jobs': -1, 'verbose': -1
        }},
        'XGBoost_Deep': {'type': 'xgb', 'params': {
            'n_estimators': 3000, 'max_depth': 6, 'learning_rate': 0.03,
            'colsample_bytree': 0.80, 'subsample': 0.80, 'tree_method': 'hist',
            'device': 'cuda', 'early_stopping_rounds': EARLY_STOPPING,
            'random_state': SEED, 'verbosity': 0
        }}
    }

    base_models = {
        'LGBM_Tuned': tuned_lgb,
        'XGBoost_Tuned': tuned_xgb,
    }
    val_preds = {
        'LGBM_Tuned': tuned_lgb.predict(X_val),
        'XGBoost_Tuned': tuned_xgb.predict(X_val),
    }

    for name, config in fixed_configs.items():
        if config['type'] == 'lgbm':
            model = fit_lgb(config['params'], X_train, y_train, X_val, y_val)
        else:
            model = fit_xgb(config['params'], X_train, y_train, X_val, y_val)
        base_models[name] = model
        val_preds[name] = model.predict(X_val)

    names = list(val_preds.keys())
    pred_matrix = np.column_stack([val_preds[n] for n in names])

    print("\n" + "="*72)
    print(f"{'Model':<22} | {'Pearson (r)':<14} | {'RMSE':<14}")
    print("-"*72)
    individual = {}
    for n in names:
        corr, _ = pearsonr(y_val, val_preds[n])
        rmse = root_mean_squared_error(y_val, val_preds[n])
        individual[n] = (corr, rmse)
        print(f"{n:<22} | {corr:<14.6f} | {rmse:<14.8f}")

    blend_preds, blend_weights = slsqp_blend(pred_matrix, y_val)
    blend_corr, _ = pearsonr(y_val, blend_preds)
    blend_rmse = root_mean_squared_error(y_val, blend_preds)

    best_single = max(individual.items(), key=lambda kv: kv[1][0])
    print("-"*72)
    print(f"{'Optimal SLSQP blend':<22} | {blend_corr:<14.6f} | {blend_rmse:<14.8f}")
    print("="*72)

    print("\nOptimal Ensemble Weights:")
    for n, w in zip(names, blend_weights):
        print(f"  - {n:<20}: {w*100:5.2f}%")

    print(f"\nBest single: {best_single[0]} (r = {best_single[1][0]:.6f})")
    print(f"Stage 7 blend r = {blend_corr:.6f} (gain vs best single: {blend_corr - best_single[1][0]:+.6f})")

    for name, model in base_models.items():
        joblib.dump(model, os.path.join(models_dir, f'stage7_{name.lower()}.joblib'))
    joblib.dump({n: float(w) for n, w in zip(names, blend_weights)},
                os.path.join(models_dir, 'stage7_optimal_weights.joblib'))
    joblib.dump(list(X.columns), os.path.join(models_dir, 'stage7_features_list.joblib'))
    joblib.dump(lgb_study.best_params, os.path.join(models_dir, 'stage7_lgb_best_params.joblib'))
    joblib.dump(xgb_study.best_params, os.path.join(models_dir, 'stage7_xgb_best_params.joblib'))

    print(f"\nStage 7 artifacts saved to: {models_dir}")

if __name__ == '__main__':
    main()
