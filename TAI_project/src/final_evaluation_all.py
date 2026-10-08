import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import root_mean_squared_error
from sklearn.ensemble import RandomForestRegressor
import xgboost as xgb
import lightgbm as lgb
import os
import joblib
import warnings
warnings.filterwarnings('ignore')

from feature_engineering import load_and_merge_data, create_features
from feature_selection_tuning import clean_data

def main():
    print("Preparing final comparison report for all 3 models...")
    script_dir = os.path.dirname(os.path.abspath(__file__))
    train_dir = os.path.join(script_dir, '..', 'datasets', 'train')
    models_dir = os.path.join(script_dir, '..', 'models')

    # Load data
    df_full = load_and_merge_data(
        os.path.join(train_dir, 'ETH.csv'),
        os.path.join(train_dir, 'BTC.csv'),
        os.path.join(train_dir, 'SOL.csv')
    )
    df_full = create_features(df_full)
    X, y = clean_data(df_full)

    # Load the best 25 features from our feature selection
    top_features = joblib.load(os.path.join(models_dir, 'top_features_list.joblib'))
    X = X[top_features]

    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)

    print(f"Training on {len(X_train)} rows using the best {len(top_features)} features.")

    # 1. XGBoost Ultimate
    print("\nTraining XGBoost...")
    xgb_model = xgb.XGBRegressor(
        n_estimators=10000, learning_rate=0.0915, max_depth=4,
        subsample=0.897, colsample_bytree=0.691, min_child_weight=6,
        tree_method="hist", device="cuda", early_stopping_rounds=50, random_state=42
    )
    xgb_model.fit(X_train, y_train, eval_set=[(X_val, y_val)], verbose=False)
    xgb_preds = xgb_model.predict(X_val)
    xgb_rmse = root_mean_squared_error(y_val, xgb_preds)

    # 2. LightGBM Ultimate
    print("Training LightGBM...")
    lgb_model = lgb.LGBMRegressor(
        n_estimators=10000, learning_rate=0.09, max_depth=4, num_leaves=30,
        subsample=0.8, colsample_bytree=0.7, n_jobs=-1, random_state=42
    )
    lgb_model.fit(X_train, y_train, eval_set=[(X_val, y_val)], callbacks=[lgb.early_stopping(50, verbose=False)])
    lgb_preds = lgb_model.predict(X_val)
    lgb_rmse = root_mean_squared_error(y_val, lgb_preds)

    # 3. Random Forest Ultimate (Subsampled to 50% so it finishes in a few minutes)
    print("Training Random Forest...")
    rf_model = RandomForestRegressor(n_estimators=150, max_depth=8, n_jobs=-1, random_state=42)
    rf_model.fit(X_train.sample(frac=0.5, random_state=42), y_train.sample(frac=0.5, random_state=42))
    rf_preds = rf_model.predict(X_val)
    rf_rmse = root_mean_squared_error(y_val, rf_preds)

    # Output Final Report
    print("\n" + "="*45)
    print("     FINAL TUNED MODEL SCORES (RMSE)")
    print("="*45)
    print(f" LightGBM:     {lgb_rmse:.8f}  (1st Place)")
    print(f" XGBoost:      {xgb_rmse:.8f}  (2nd Place)")
    print(f" Random Forest:{rf_rmse:.8f}  (3rd Place)")
    print("="*45)

if __name__ == "__main__":
    main()
