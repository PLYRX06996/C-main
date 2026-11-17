# 5. System Implementation and Quantitative Terminal

## 5.1 Software Architecture and Reproducibility Pipeline
The computational framework is structured as a modular, reproducible research pipeline designed to transition seamlessly from exploratory batch training to out-of-sample evaluation and deployment. 

The end-to-end architecture (illustrated in Figure F9) follows a unidirectional data flow:
1. **Ingestion & Data Hygiene:** Ingests raw Level-2 market feeds from `datasets/train/` (ETH, BTC, SOL), applies price feed sanitation, and performs continuous forward filling.
2. **Feature Extraction:** Computes 135 continuous multi-scale volatility, microstructure, and cross-asset indicators using vectorized NumPy and Pandas routines.
3. **Model Estimation:** Executes model training with hardware acceleration (CUDA histogram binning for XGBoost, multi-threaded C++ for LightGBM, and optimized LAPACK for scikit-learn RidgeCV).
4. **Artifact Serialization:** Serializes fitted estimators and precomputed validation/test prediction matrices via `joblib`.
5. **Interactive Terminal Interface:** Delivers a lightweight Streamlit dashboard for real-time order book inspection and risk visualization.

## 5.2 Stage-to-Script and Artifact Mapping
Every experimental stage is implemented in an independent, executable Python script within `src/`, with corresponding serialized artifacts stored in `models/`:

| Stage | Script Name | Key Artifact Generated | Objective & Key Metric |
|---|---|---|---|
| Stage 1 | `stage1_train_baselines.py` | `models/lgbm_baseline.pkl`, `xgb_baseline.pkl` | Raw L2 order book benchmarks ($r \approx 0.09$) |
| Stage 1 | `stage1_overfitting_proof.py` | Console log evidence | 1,000-tree overfitting demonstration (RMSE doubled) |
| Stage 2 | `stage2_cross_asset_features.py` | `models/stage2_best_model.joblib` | v2 microstructure & spread features ($r = 0.6442$) |
| Stage 2 | `stage2_pca_vs_tree_reduction.py` | Console log evidence | PCA vs supervised tree reduction ($0.5345$ vs $0.6435$) |
| Stage 3 | `stage3_feature_selection_tuning.py`| `models/top_features_list.joblib` | Top-25 feature selection via Optuna TPE |
| Stage 3 | `stage3_advanced_tuning.py` | `models/tuned_feature_selected_xgboost.joblib` | Extended Bayesian hyperparameter optimization |
| Stage 4 | `stage4_multiscale_pipeline.py` | `models/stage4_multiscale_best.joblib` | Continuous multi-scale EWMA pipeline ($r = 0.6851$) |
| Stage 5 | `stage5_model_ensemble.py` | `models/stage5_lgbm_conservative.joblib`, etc. | SLSQP constrained tree ensemble ($r = 0.6910$) |
| Stage 6 | `stage6_stacking_ensemble.py` | Console log evidence | Stacking meta-learner benchmark (Stacking fails) |
| Stage 7 | `stage7_optuna_ensemble.py` | Console log evidence | Optuna Pearson-r tuned ensemble ($r = 0.6914$) |
| Stage 8 | `stage8_har_rv_timeseries.py` | `models/stage8_har_rv_results.csv` | Corsi HAR-RV with Newey-West HAC ($r = 0.6860$) |
| Benchmark | `eval_rigorous_test_benchmark.py` | `models/rigorous_benchmark_3way_results.csv` | Authoritative 70/15/15 3-way test ($r = 0.6887$) |
| Terminal | `app.py` | `data/processed/app_cache.joblib` | Real-time Streamlit microstructure terminal |

## 5.3 High-Performance Caching and Export Pipeline
To decouple real-time UI rendering from heavyweight model training and data merging, `src/eval_rigorous_test_benchmark.py` incorporates an automated export switch: `--export-app`.

When executed with this flag, the script performs three critical serialization tasks:
1. **CPU Model Parameter Conversion:** Calls `xgb_cons.set_params(device='cpu')` on the CUDA-trained XGBoost model before serialization. This guarantees that the exported model loads and executes on standard CPU hardware without requiring CUDA drivers or dedicated GPUs.
2. **Model Persistence:** Serializes five self-contained joblib objects into `models/`:
   - `final_hybrid_ridge.joblib` (fitted RidgeCV pipeline with StandardScaler)
   - `final_hybrid_lgbm.joblib` (fitted LightGBM Conservative model)
   - `final_hybrid_xgb.joblib` (fitted CPU-native XGBoost Conservative model)
   - `final_feature_list.joblib` (135 feature column names)
   - `final_vol_cols.joblib` (30 volatility feature column names)
3. **Compressed Application Cache:** Compresses precomputed validation and test predictions into `data/processed/app_cache.joblib` (`compress=3`, size: 49.27 MB):
   - `'meta'`: Slice lengths, horizon text, training label percentiles ($q_{33}, q_{67}$), and timestamp metadata.
   - `'val'` and `'test'`: Full DataFrames with 135 float32 features, ground truth labels, and predictions for Persistence, Ridge, LightGBM, XGBoost, Tree Blend, and Hybrid.
   - `'metrics'`: Precomputed Pearson $r$ and RMSE metrics for instant UI table display.

## 5.4 Interactive Streamlit Microstructure Terminal
The web interface (`app.py`, 229 lines) provides institutional users with an interactive quantitative terminal:
- **Depth Chart Visualizer:** Dynamically plots 5-level cumulative bid (green) and ask (red) depth curves with a dashed vertical line at the mid-price, using step `hv` interpolation and fixed axis bounds to prevent jitter.
- **Microstructure Bar:** Displays mid-price, inside spread (absolute and basis points), and Order Book Imbalances ($\text{OBI}_1, \text{OBI}_2, \text{OBI}_3$).
- **Forward Volatility Risk Gauge:** Maps predicted hybrid volatility into an institutional three-color risk dial (Green: Low $\le q_{33}$, Yellow: Medium $\le q_{67}$, Red: High $> q_{67}$), parameterized strictly by training set percentiles.
- **Local Context Trajectory:** Renders a fast WebGL time-series chart (`Scattergl`) showing actual volatility against model forecasts over an adjustable context window (100 to 2,000 ticks) around the cursor.
- **Live Inference Consistency Check:** On every selected snapshot, the terminal executes live forward inference using the three serialized models and verifies that the maximum absolute drift versus the precomputed cache is strictly below $10^{-6}$ (`PASS`).

## 5.5 Automated Test Suite and Quality Assurance
To satisfy Trustworthy AI engineering standards, the codebase incorporates an automated pytest test harness in `tests/`:
1. `tests/test_app_artifacts.py`: Executes 8 data integrity tests in under 3 seconds:
   - Cache dictionary structure and required keys.
   - Exact slice row count verification ($n_{\text{val}} = 90,890$, $n_{\text{test}} = 90,892$).
   - Strict chronological ordering (`row_id` strictly monotonic, timestamps non-decreasing, no partition overlap).
   - Absolute absence of NaN or infinite floating-point values.
   - Exact floating-point metric reproduction ($|r_{\text{recomputed}} - r_{\text{cached}}| < 10^{-9}$).
   - Live model inference validation on 50 randomized samples ($|\text{live} - \text{cached}| < 10^{-6}$).
   - Valid volatility regime threshold hierarchy ($0 < q_{33} < q_{67}$).
   - Verification of test hybrid Pearson $r$ within target bounds ($0.6887 \pm 0.002$).
2. `tests/test_app_smoke.py`: Uses Streamlit's official `AppTest` framework to simulate headless user navigation, partition toggling, and slider adjustments, verifying zero unhandled exceptions.

## 5.6 Computational Runtime and Hardware Specifications
All batch training and verification experiments were executed on a dedicated Linux workstation. The complete 19-script batch pipeline (`run_all.sh`) finishes in **1,077 seconds (~18.0 minutes)**, demonstrating exceptional computational efficiency:

| Computational Task | Runtime | Hardware Acceleration |
|---|---|---|
| Stage 1 Baselines & Overfitting Proof | 42 s | Multi-threaded CPU |
| Stage 2 Cross-Asset & Feature Reduction | 115 s | CUDA Hist Binning (RTX 2050) |
| Stage 3 Optuna Bayesian Search (60 Trials) | 285 s | CUDA Hist Binning (RTX 2050) |
| Stage 4 Continuous EWMA Pipeline | 88 s | CUDA Hist Binning (RTX 2050) |
| Stage 5 Ensembling & SLSQP Optimization | 142 s | CUDA Hist + SciPy SLSQP |
| Stage 6 Stacking Meta-Learner Benchmark | 95 s | CUDA Hist + CPU Ridge |
| Stage 7 Optuna Pearson-r Ensemble Tuning | 160 s | CUDA Hist (RTX 2050) |
| Stage 8 Econometric HAR-RV with HAC | 24 s | Multi-threaded LAPACK / NumPy |
| Authoritative 3-Way Benchmark & Export | 78 s | CUDA Hist + CPU Export |
| Automated Pytest Test Harness | 4 s | CPU (Pytest 9.2) |
| **Total Pipeline Execution** | **1,077 s** | **NVIDIA RTX 2050 (4GB VRAM) + AMD Ryzen** |
