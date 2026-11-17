"""
Unit and Integrity Tests for Streamlit App Artifacts
=====================================================
Validates data integrity, chronological consistency, model export fidelity,
and reproducibility of cached benchmark predictions without retraining.
"""

import os
import pytest
import joblib
import numpy as np
import pandas as pd
from scipy.stats import pearsonr

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CACHE_PATH = os.path.join(ROOT_DIR, 'data', 'processed', 'app_cache.joblib')
MODELS_DIR = os.path.join(ROOT_DIR, 'models')


@pytest.fixture(scope="module")
def cache():
    assert os.path.exists(CACHE_PATH), f"App cache missing at {CACHE_PATH}. Run benchmark with --export-app."
    return joblib.load(CACHE_PATH)


def test_cache_structure(cache):
    """Test 1: App cache loads and contains required primary keys."""
    expected_keys = {'meta', 'val', 'test', 'metrics'}
    assert expected_keys.issubset(set(cache.keys())), f"Cache missing keys: {expected_keys - set(cache.keys())}"


def test_slice_lengths(cache):
    """Test 2: Validation and test slice lengths match metadata definitions."""
    n_val = cache['meta']['n_val']
    n_test = cache['meta']['n_test']
    assert len(cache['val']) == n_val == 90890, f"Validation length mismatch: {len(cache['val'])} vs {n_val}"
    assert len(cache['test']) == n_test == 90892, f"Test length mismatch: {len(cache['test'])} vs {n_test}"


def test_chronology(cache):
    """Test 3: Temporal order is strictly preserved with no overlap."""
    val_rows = cache['val']['row_id'].values
    test_rows = cache['test']['row_id'].values

    # Row IDs strictly increasing
    assert np.all(np.diff(val_rows) > 0), "Validation row_id is not strictly increasing"
    assert np.all(np.diff(test_rows) > 0), "Test row_id is not strictly increasing"

    # Strictly disjoint partitions (train < val < test)
    assert val_rows.max() < test_rows.min(), f"Validation overlaps with test: {val_rows.max()} >= {test_rows.min()}"

    # Timestamps non-decreasing
    val_ts = pd.to_datetime(cache['val']['timestamp'])
    test_ts = pd.to_datetime(cache['test']['timestamp'])
    assert np.all(np.diff(val_ts.astype('int64')) >= 0), "Validation timestamps contain temporal retrogressions"
    assert np.all(np.diff(test_ts.astype('int64')) >= 0), "Test timestamps contain temporal retrogressions"


def test_no_nan_or_inf(cache):
    """Test 4: Engineered features and model predictions contain no NaN or Infinite values."""
    for split_name in ['val', 'test']:
        df = cache[split_name]
        numeric_cols = df.select_dtypes(include=[np.number]).columns
        nan_counts = df[numeric_cols].isna().sum().sum()
        inf_counts = np.isinf(df[numeric_cols].to_numpy()).sum()
        assert nan_counts == 0, f"Found {nan_counts} NaNs in {split_name} split"
        assert inf_counts == 0, f"Found {inf_counts} Infs in {split_name} split"


def test_recomputed_pearson_r_matches_metrics(cache):
    """Test 5: Recomputed Pearson r matches cached metrics to floating-point precision."""
    pred_cols = ['pred_persistence', 'pred_ridge', 'pred_lgbm', 'pred_xgb', 'pred_tree_blend', 'pred_hybrid']
    for split_name in ['val', 'test']:
        df = cache[split_name]
        y_true = df['label'].values
        for col in pred_cols:
            r_recomputed, _ = pearsonr(y_true, df[col].values)
            r_stored = cache['metrics'][split_name][col]['r']
            assert abs(r_recomputed - r_stored) < 1e-9, (
                f"Pearson r mismatch on {split_name} {col}: recomputed {r_recomputed} vs stored {r_stored}"
            )


def test_live_inference_matches_cached_predictions(cache):
    """Test 6: Live inference with saved models on 50 samples matches cached predictions within 1e-6."""
    ridge_model = joblib.load(os.path.join(MODELS_DIR, 'final_hybrid_ridge.joblib'))
    lgbm_model = joblib.load(os.path.join(MODELS_DIR, 'final_hybrid_lgbm.joblib'))
    xgb_model = joblib.load(os.path.join(MODELS_DIR, 'final_hybrid_xgb.joblib'))
    feature_list = joblib.load(os.path.join(MODELS_DIR, 'final_feature_list.joblib'))
    vol_cols = joblib.load(os.path.join(MODELS_DIR, 'final_vol_cols.joblib'))

    test_df = cache['test']
    sample = test_df.sample(n=50, random_state=42)

    X_full = sample[feature_list]
    X_vol = sample[vol_cols]

    live_ridge = ridge_model.predict(X_vol)
    live_lgbm = lgbm_model.predict(X_full)
    live_xgb = xgb_model.predict(X_full)

    np.testing.assert_allclose(live_ridge, sample['pred_ridge'].values, atol=1e-6, err_msg="Ridge live mismatch")
    np.testing.assert_allclose(live_lgbm, sample['pred_lgbm'].values, atol=1e-6, err_msg="LightGBM live mismatch")
    np.testing.assert_allclose(live_xgb, sample['pred_xgb'].values, atol=1e-6, err_msg="XGBoost live mismatch")


def test_percentile_order(cache):
    """Test 7: Volatility regime thresholds maintain valid monotonic hierarchy."""
    q33 = cache['meta']['q33']
    q67 = cache['meta']['q67']
    assert 0.0 < q33 < q67, f"Invalid regime percentiles: q33={q33}, q67={q67}"


def test_hybrid_pearson_r_bounds(cache):
    """Test 8: Final held-out test hybrid Pearson r reproduces verified benchmark target."""
    test_hybrid_r = cache['metrics']['test']['pred_hybrid']['r']
    assert test_hybrid_r == pytest.approx(0.6887, abs=2e-3), (
        f"Test hybrid r {test_hybrid_r:.4f} outside target bounds [0.6867, 0.6907]"
    )
