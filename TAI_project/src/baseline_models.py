import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import root_mean_squared_error, mean_absolute_error
import xgboost as xgb
import lightgbm as lgb
import time
import joblib
import os

def load_and_prep_data(filepath):
    print(f"Loading data from {filepath}...")
    df = pd.read_csv(filepath)

    # Replace infinities with NaN so they get dropped
    df = df.replace([np.inf, -np.inf], np.nan)

    # Drop rows with NaN values
    initial_len = len(df)
    df = df.dropna()
    print(f"Dropped {initial_len - len(df)} NaN/Inf rows.")

    # We will exclude timestamp for now as a feature to just use order book state
    features = [c for c in df.columns if c not in ['timestamp', 'label']]

    # Cast features to float32 to catch overflow values (they become inf)
    # Then replace inf with nan and drop them
    X = df[features].astype(np.float32)
    X = X.replace([np.inf, -np.inf], np.nan)

    # Check if there are any remaining NaNs in X, and drop those rows from both X and y
    valid_idx = X.dropna().index
    X = X.loc[valid_idx]
    y = df.loc[valid_idx, 'label']

    print(f"Kept {len(X)} valid rows for training/validation.")

    return X, y

def main():
    eth_train_path = "../Stock Market Prediction Analysis/train/ETH.csv"

    # Using relative path from the script location
    import os
    script_dir = os.path.dirname(os.path.abspath(__file__))
    eth_train_path = os.path.join(script_dir, '..', 'Stock Market Prediction Analysis', 'train', 'ETH.csv')

    X, y = load_and_prep_data(eth_train_path)

    # Time series data, so we don't shuffle for validation split
    # Let's use 80% for training and 20% for validation
    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)

    print(f"Training set: {X_train.shape[0]} rows")
    print(f"Validation set: {X_val.shape[0]} rows")

    # Baseline 1: Random Forest
    print("\n--- Training Random Forest ---")
    start_time = time.time()
    # Using limited max_depth and n_estimators to keep training time reasonable for a baseline
    rf = RandomForestRegressor(n_estimators=50, max_depth=10, n_jobs=-1, random_state=42)
    rf.fit(X_train, y_train)
    rf_time = time.time() - start_time

    rf_preds = rf.predict(X_val)
    rf_rmse = root_mean_squared_error(y_val, rf_preds)
    rf_mae = mean_absolute_error(y_val, rf_preds)
    print(f"Random Forest Time: {rf_time:.2f}s")
    print(f"Random Forest RMSE: {rf_rmse:.6f}")
    print(f"Random Forest MAE:  {rf_mae:.6f}")
    joblib.dump(rf, os.path.join(script_dir, '..', 'models', 'random_forest_baseline.joblib'))

    # Baseline 2: XGBoost
    print("\n--- Training XGBoost ---")
    start_time = time.time()
    # Basic params
    xgb_model = xgb.XGBRegressor(n_estimators=100, learning_rate=0.1, max_depth=6, n_jobs=-1, random_state=42)
    xgb_model.fit(X_train, y_train, eval_set=[(X_val, y_val)], verbose=False)
    xgb_time = time.time() - start_time

    xgb_preds = xgb_model.predict(X_val)
    xgb_rmse = root_mean_squared_error(y_val, xgb_preds)
    xgb_mae = mean_absolute_error(y_val, xgb_preds)
    print(f"XGBoost Time: {xgb_time:.2f}s")
    print(f"XGBoost RMSE: {xgb_rmse:.6f}")
    print(f"XGBoost MAE:  {xgb_mae:.6f}")
    joblib.dump(xgb_model, os.path.join(script_dir, '..', 'models', 'xgboost_baseline.joblib'))

    # Baseline 3: LightGBM
    print("\n--- Training LightGBM ---")
    start_time = time.time()
    lgb_model = lgb.LGBMRegressor(n_estimators=100, learning_rate=0.1, max_depth=6, n_jobs=-1, random_state=42)
    lgb_model.fit(X_train, y_train, eval_set=[(X_val, y_val)])
    lgb_time = time.time() - start_time

    lgb_preds = lgb_model.predict(X_val)
    lgb_rmse = root_mean_squared_error(y_val, lgb_preds)
    lgb_mae = mean_absolute_error(y_val, lgb_preds)
    print(f"LightGBM Time: {lgb_time:.2f}s")
    print(f"LightGBM RMSE: {lgb_rmse:.6f}")
    print(f"LightGBM MAE:  {lgb_mae:.6f}")
    joblib.dump(lgb_model, os.path.join(script_dir, '..', 'models', 'lightgbm_baseline.joblib'))

if __name__ == "__main__":
    main()
