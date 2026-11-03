"""
Stage 5: Multi-Model Gradient Boosting Ensemble (LightGBM & XGBoost)
=====================================================================
Empirical Objective:
Following mentor direction:
1. Random Forest is dropped from the active pipeline due to negative directional
   correlation (-0.0189) and computational inefficiencies on high-frequency data.
2. Trains a diverse suite of gradient-boosted decision tree models across LightGBM
   and XGBoost architectures, varying inductive biases (tree depth, feature subsampling,
   and loss functions).
3. Evaluates linear prediction ensembling using both equal weighting and constrained
   numerical weight optimization (SLSQP) to determine whether combining diverse learners
   enhances out-of-sample directional forecasting (Pearson r) over single-model estimators.
"""

import os
import joblib
import warnings
import numpy as np
import pandas as pd
from scipy.stats import pearsonr
from scipy.optimize import minimize
from sklearn.model_selection import train_test_split
from sklearn.metrics import root_mean_squared_error
import lightgbm as lgb
import xgboost as xgb

from stage4_champion_pipeline import load_and_merge, engineer_champion_features, clean_and_prepare

warnings.filterwarnings('ignore', category=FutureWarning)
warnings.filterwarnings('ignore', category=UserWarning)

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    if not os.path.exists(train_dir):
        train_dir = os.path.join(script_dir, '..', 'data', 'raw', 'train')
    models_dir = os.path.join(script_dir, '..', 'models')
    os.makedirs(models_dir, exist_ok=True)

    print(" Stage 5: Multi-Model Ensemble Pipeline (LightGBM + XGBoost)")

    df = load_and_merge(train_dir)
    df = engineer_champion_features(df)
    X, y = clean_and_prepare(df)

    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)
    print(f"\nDataset Dimensions: {len(X_train)} Train rows | {len(X_val)} Validation rows | {X.shape[1]} Features\n")

    model_configs = {
        'LGBM_Conservative': {
            'type': 'lgbm',
            'params': {
                'n_estimators': 3000,
                'max_depth': 4,
                'num_leaves': 15,
                'learning_rate': 0.05,
                'colsample_bytree': 0.70,
                'subsample': 0.80,
                'random_state': 42,
                'n_jobs': -1
            }
        },
        'LGBM_Deep': {
            'type': 'lgbm',
            'params': {
                'n_estimators': 3000,
                'max_depth': 6,
                'num_leaves': 40,
                'learning_rate': 0.03,
                'colsample_bytree': 0.80,
                'subsample': 0.80,
                'random_state': 42,
                'n_jobs': -1
            }
        },
        'LGBM_Huber': {
            'type': 'lgbm',
            'params': {
                'objective': 'huber',
                'n_estimators': 3000,
                'max_depth': 6,
                'num_leaves': 40,
                'learning_rate': 0.04,
                'colsample_bytree': 0.60,
                'subsample': 0.85,
                'random_state': 42,
                'n_jobs': -1
            }
        },
        'XGBoost_Conservative': {
            'type': 'xgb',
            'params': {
                'n_estimators': 3000,
                'max_depth': 4,
                'learning_rate': 0.05,
                'colsample_bytree': 0.60,
                'subsample': 0.80,
                'tree_method': 'hist',
                'device': 'cuda',
                'early_stopping_rounds': 50,
                'random_state': 42
            }
        },
        'XGBoost_Deep': {
            'type': 'xgb',
            'params': {
                'n_estimators': 3000,
                'max_depth': 6,
                'learning_rate': 0.03,
                'colsample_bytree': 0.80,
                'subsample': 0.80,
                'tree_method': 'hist',
                'device': 'cuda',
                'early_stopping_rounds': 50,
                'random_state': 42
            }
        }
    }

    trained_models = {}
    val_predictions = {}
    individual_metrics = {}

    print("--- Training Constituent Ensemble Models ---")
    for name, config in model_configs.items():
        print(f"Training {name}...", flush=True)
        if config['type'] == 'lgbm':
            model = lgb.LGBMRegressor(**config['params'])
            model.fit(
                X_train, y_train,
                eval_set=[(X_val, y_val)],
                callbacks=[lgb.early_stopping(50, verbose=False)]
            )
            preds = model.predict(X_val)
            stopped_iter = model.best_iteration_
        else:
            model = xgb.XGBRegressor(**config['params'])
            model.fit(
                X_train, y_train,
                eval_set=[(X_val, y_val)],
                verbose=False
            )
            model.set_params(device='cpu')
            preds = model.predict(X_val)
            stopped_iter = model.best_iteration

        corr, _ = pearsonr(y_val, preds)
        rmse = root_mean_squared_error(y_val, preds)
        trained_models[name] = model
        val_predictions[name] = preds
        individual_metrics[name] = (corr, rmse, stopped_iter)

    print("\n" + "="*70)
    print(f"{'Model Configuration':<24} | {'Pearson (r)':<14} | {'RMSE':<14} | {'Best Iter':<10}")
    print("="*70)
    best_single_name = None
    best_single_corr = -1.0
    for name, (corr, rmse, stopped_iter) in individual_metrics.items():
        print(f"{name:<24} | {corr:<14.6f} | {rmse:<14.8f} | {stopped_iter:<10}")
        if corr > best_single_corr:
            best_single_corr = corr
            best_single_name = name
    print("="*70)
    print(f"Top Individual Model: {best_single_name} (r = {best_single_corr:.6f})")

    pred_names = list(val_predictions.keys())
    pred_matrix = np.column_stack([val_predictions[n] for n in pred_names])

    equal_weights = np.ones(len(pred_names)) / len(pred_names)
    equal_blend_preds = np.dot(pred_matrix, equal_weights)
    equal_corr, _ = pearsonr(y_val, equal_blend_preds)
    equal_rmse = root_mean_squared_error(y_val, equal_blend_preds)

    def objective_fn(weights):
        normalized_w = weights / np.sum(weights)
        combined = np.dot(pred_matrix, normalized_w)
        return -pearsonr(y_val, combined)[0]

    initial_weights = np.ones(len(pred_names)) / len(pred_names)
    weight_bounds = [(0.0, 1.0) for _ in range(len(pred_names))]
    opt_result = minimize(objective_fn, initial_weights, bounds=weight_bounds, method='SLSQP')
    optimal_weights = opt_result.x / np.sum(opt_result.x)

    optimal_blend_preds = np.dot(pred_matrix, optimal_weights)
    optimal_corr, _ = pearsonr(y_val, optimal_blend_preds)
    optimal_rmse = root_mean_squared_error(y_val, optimal_blend_preds)

    print("\n" + "="*70)
    print("                  ENSEMBLE EVALUATION SUMMARY")
    print("="*70)
    print(f"{'Strategy':<26} | {'Pearson (r)':<14} | {'RMSE':<14} | {'Gain over Single':<16}")
    print("-"*70)
    print(f"{'Best Single Model':<26} | {best_single_corr:<14.6f} | {individual_metrics[best_single_name][1]:<14.8f} | {'Baseline':<16}")
    equal_gain = equal_corr - best_single_corr
    print(f"{'Equal-Weight Blend':<26} | {equal_corr:<14.6f} | {equal_rmse:<14.8f} | {equal_gain:+14.6f}")
    opt_gain = optimal_corr - best_single_corr
    print(f"{'Optimized-Weight Blend':<26} | {optimal_corr:<14.6f} | {optimal_rmse:<14.8f} | {opt_gain:+14.6f}")
    print("="*70)

    print("\nOptimal Ensemble Weights Distribution:")
    for name, w in zip(pred_names, optimal_weights):
        print(f"  - {name:<22}: {w*100:5.2f}%")

    for name, model in trained_models.items():
        joblib.dump(model, os.path.join(models_dir, f'stage5_{name.lower()}.joblib'))
    
    weights_dict = {name: float(w) for name, w in zip(pred_names, optimal_weights)}
    joblib.dump(weights_dict, os.path.join(models_dir, 'stage5_optimal_weights.joblib'))
    joblib.dump(list(X.columns), os.path.join(models_dir, 'stage5_features_list.joblib'))

    print(f"\nTrained models and optimal weights saved to: {models_dir}")

if __name__ == '__main__':
    main()
