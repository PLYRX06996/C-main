import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import root_mean_squared_error, mean_absolute_error
import xgboost as xgb
import lightgbm as lgb
import time
import os
import joblib

def load_and_prep_data(filepath):
    print(f"Loading data from {filepath}...")
    df = pd.read_csv(filepath)

    features = [c for c in df.columns if c not in ['timestamp', 'label']]

    # Cast to float32 to prevent overflow, replace infs with nan
    X = df[features].astype(np.float32)
    y = df['label'].astype(np.float32)

    X = X.replace([np.inf, -np.inf], np.nan)
    y = y.replace([np.inf, -np.inf], np.nan)

    # We need to drop rows where either X has a NaN OR y has a NaN
    mask = X.notna().all(axis=1) & y.notna()
    X = X[mask]
    y = y[mask]

    return X, y

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    eth_train_path = os.path.join(script_dir, '..', 'Stock Market Prediction Analysis', 'train', 'ETH.csv')

    X, y = load_and_prep_data(eth_train_path)

    # 80-20 Time Series Split
    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)
    print(f"Training set: {X_train.shape[0]} rows | Validation set: {X_val.shape[0]} rows\n")

    models_dir = os.path.join(script_dir, '..', 'models')
    os.makedirs(models_dir, exist_ok=True)

    # Note: We skip Random Forest here because it doesn't train sequentially
    # (each tree is independent), so "early stopping" doesn't apply to it
    # in the same way it does to Boosting models.

    print("--- Training XGBoost (with GPU & Early Stopping) ---")
    start_time = time.time()

    # We give it up to 10,000 trees, but ask it to stop if the validation score
    # hasn't improved for 50 rounds straight!
    xgb_model = xgb.XGBRegressor(
        n_estimators=10000,
        learning_rate=0.05,
        max_depth=6,
        random_state=42,
        tree_method="hist",
        device="cuda",         # Using your NVIDIA RTX GPU!
        early_stopping_rounds=50
    )

    # Notice we pass the validation set in the fit function for early stopping
    xgb_model.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        verbose=100  # Print progress every 100 trees
    )
    xgb_time = time.time() - start_time

    print(f"-> XGBoost stopped at tree number {xgb_model.best_iteration}")
    xgb_preds = xgb_model.predict(X_val)
    xgb_rmse = root_mean_squared_error(y_val, xgb_preds)
    print(f"XGBoost Total Time: {xgb_time:.2f}s")
    print(f"XGBoost Best RMSE: {xgb_rmse:.6f}")
    joblib.dump(xgb_model, os.path.join(models_dir, 'xgboost_early_stop.joblib'))

    print("\n--- Training LightGBM (with CPU & Early Stopping) ---")
    start_time = time.time()

    # LightGBM via CPU (no OpenCL device setup needed, it's very fast regardless)
    lgb_model = lgb.LGBMRegressor(
        n_estimators=10000,
        learning_rate=0.05,
        max_depth=6,
        n_jobs=-1,
        random_state=42
    )

    # Use callbacks for early stopping in lightgbm
    lgb_model.fit(
        X_train, y_train,
        eval_set=[(X_val, y_val)],
        callbacks=[
            lgb.early_stopping(stopping_rounds=50, verbose=True),
            lgb.log_evaluation(period=100)
        ]
    )
    lgb_time = time.time() - start_time

    print(f"-> LightGBM stopped at tree number {lgb_model.best_iteration_}")
    lgb_preds = lgb_model.predict(X_val)
    lgb_rmse = root_mean_squared_error(y_val, lgb_preds)
    print(f"LightGBM Total Time: {lgb_time:.2f}s")
    print(f"LightGBM Best RMSE: {lgb_rmse:.6f}")
    joblib.dump(lgb_model, os.path.join(models_dir, 'lightgbm_early_stop.joblib'))

if __name__ == "__main__":
    main()
