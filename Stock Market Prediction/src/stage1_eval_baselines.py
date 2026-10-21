import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import root_mean_squared_error
from scipy.stats import pearsonr
import joblib
import os
import warnings

warnings.filterwarnings('ignore')

def load_and_prep_data(filepath):
    df = pd.read_csv(filepath)
    features = [c for c in df.columns if c not in ['timestamp', 'label']]
    

    X = df[features].astype(np.float32)
    y = df['label'].astype(np.float32)
    
    X = X.replace([np.inf, -np.inf], np.nan)
    y = y.replace([np.inf, -np.inf], np.nan)
    
    mask = X.notna().all(axis=1) & y.notna()
    return X[mask], y[mask]

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    eth_train_path = os.path.join(script_dir, '..', 'datasets', 'train', 'ETH.csv')
    

    if not os.path.exists(eth_train_path):
        eth_train_path = os.path.join(script_dir, '..', 'data', 'raw', 'train', 'ETH.csv')

    print("Loading raw ETH data for baseline evaluation...")
    X, y = load_and_prep_data(eth_train_path)

    X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, shuffle=False)
    print(f"Validation set size: {len(X_val)} rows\n")

    models_dir = os.path.join(script_dir, '..', 'models')

    models = {
        'Random Forest (Raw)': 'random_forest_baseline.joblib',
        'XGBoost (Raw)': 'xgboost_baseline.joblib',
        'LightGBM (Raw)': 'lightgbm_baseline.joblib'
    }

    print("="*65)
    print(f"{'Model (Baseline)':<22} | {'RMSE':<12} | {'Pearson Correlation (r)':<25}")
    print("="*65)

    for model_name, filename in models.items():
        model_path = os.path.join(models_dir, filename)
        if os.path.exists(model_path):
            try:
                model = joblib.load(model_path)
                preds = model.predict(X_val)
                
                rmse = root_mean_squared_error(y_val, preds)

                correlation, p_value = pearsonr(y_val, preds)
                
                print(f"{model_name:<22} | {rmse:<12.8f} | {correlation:<12.6f} (p-val: {p_value:.1e})")
            except Exception as e:
                print(f"{model_name:<22} | {'Error Loading Model':<12} | {str(e)}")
        else:
            print(f"{model_name:<22} | {'N/A (Not Found)':<12} | {'N/A'}")

    print("="*65)
    print("\n* Note: Pearson correlation (r) measures linear directional association.")
    print("  Higher positive values indicate better tracking of volatility regimes.")

if __name__ == "__main__":
    main()
