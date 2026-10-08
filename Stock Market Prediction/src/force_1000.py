import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import root_mean_squared_error, mean_absolute_error
import xgboost as xgb
import lightgbm as lgb
import time
import os

def load_and_prep_data(filepath):
    df = pd.read_csv(filepath)
    features = [c for c in df.columns if c not in ['timestamp', 'label']]
    X = df[features].astype(np.float32)
    y = df['label'].astype(np.float32)

    X = X.replace([np.inf, -np.inf], np.nan)
    y = y.replace([np.inf, -np.inf], np.nan)

    mask = X.notna().all(axis=1) & y.notna()
    X = X[mask]
    y = y[mask]
    return X, y

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    eth_train_path = os.path.join(script_dir, '..', 'data', 'raw', 'train', 'ETH.csv')

    X, y = load_and_prep_data(eth_train_path)
    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)

    print("WARNING: Forcing 1000 iterations on all models!")

    print("\n--- Training XGBoost (1000 Trees, GPU) ---")
    xgb_model = xgb.XGBRegressor(n_estimators=1000, learning_rate=0.05, max_depth=6, tree_method="hist", device="cuda", random_state=42)
    xgb_model.fit(X_train, y_train)
    xgb_preds = xgb_model.predict(X_val)
    print(f"XGBoost Validation RMSE: {root_mean_squared_error(y_val, xgb_preds):.6f}")

    print("\n--- Training LightGBM (1000 Trees, CPU) ---")
    lgb_model = lgb.LGBMRegressor(n_estimators=1000, learning_rate=0.05, max_depth=6, n_jobs=-1, random_state=42)
    lgb_model.fit(X_train, y_train)
    lgb_preds = lgb_model.predict(X_val)
    print(f"LightGBM Validation RMSE: {root_mean_squared_error(y_val, lgb_preds):.6f}")

    sample_size = int(len(X_train) * 0.3)
    X_train_sub = X_train.iloc[:sample_size]
    y_train_sub = y_train.iloc[:sample_size]

    print("\n--- Training Random Forest (1000 Trees) ---")
    print("(Using 30% of data to keep training time under ~3 minutes)")
    start = time.time()
    rf = RandomForestRegressor(n_estimators=1000, max_depth=10, n_jobs=-1, random_state=42)
    rf.fit(X_train_sub, y_train_sub)
    rf_preds = rf.predict(X_val)
    print(f"Random Forest Time: {time.time()-start:.2f}s")
    print(f"Random Forest Validation RMSE: {root_mean_squared_error(y_val, rf_preds):.6f}")

if __name__ == "__main__":
    main()
