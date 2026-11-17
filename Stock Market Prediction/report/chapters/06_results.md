# 6. Empirical Results and Benchmark Analysis

## 6.1 Raw Level-2 Baselines and Overfitting Proof
To test whether raw limit order book depth provides direct predictive utility for forward volatility, Stage 1 trained Random Forest, XGBoost, and LightGBM models directly on the 21 unengineered ETH features.

| Model / Configuration | Features | Early Stop Round | Pearson $r$ | Validation RMSE |
|---|---|---|---|---|
| Random Forest (Scikit-Learn) | 21 Raw Depth | None | -0.0189 | $9.82 \times 10^{-5}$ |
| XGBoost Default | 21 Raw Depth | Round 5 | +0.0887 | $7.91 \times 10^{-5}$ |
| LightGBM Default | 21 Raw Depth | Round 6 | +0.0947 | $7.90 \times 10^{-5}$ |
| XGBoost Forced 1,000 Trees | 21 Raw Depth | Forced 1,000 | +0.0612 | $13.20 \times 10^{-5}$ (+67%) |
| LightGBM Forced 1,000 Trees | 21 Raw Depth | Forced 1,000 | +0.0458 | $16.40 \times 10^{-5}$ (+108%) |

As visualized in Figure F2, the empirical conclusions are striking:
1. **Negligible Signal:** Raw depth levels yield near-zero correlation ($r \approx 0.09$), confirming that static order book snapshots without temporal context cannot anticipate forward variance.
2. **Instantaneous Overfitting:** Gradient boosted trees hit their validation optimum within just 5 to 6 boosting iterations.
3. **Catastrophic Noise Memorization:** Disabling early stopping and forcing trees to grow to 1,000 rounds doubles validation RMSE (inflating from $7.90 \times 10^{-5}$ to $16.40 \times 10^{-5}$ in LightGBM), providing conclusive mathematical proof of rapid overfitting to market microstructure noise.

## 6.2 Feature Engineering Progression (Selection-Biased Validation)
Across Stages 2 through 7, domain-specific feature engineering transformed model performance. The progression of validation Pearson correlation (evaluated under the exploratory 80/20 chronological split) is summarized below and depicted in Figure F3:

| Stage | Pipeline Milestone | Feature Space | Validation $r$ | Primary Signal Driver |
|---|---|---|---|---|
| Stage 1 | Raw Baseline Benchmarks | 21 Raw Depth | 0.0947 | Raw LOB price/volume depth |
| Stage 2 | Microstructure & Spreads (v2) | 90 Features | 0.6442 | Realized volatility & depth spreads |
| Stage 3 | Bayesian Optuna Tuning (v3) | 97 Features | 0.6558 | Hyperparameter & lag refinement |
| Stage 4 | Continuous Multi-Scale EWMA | 135 Features | 0.6851 | RiskMetrics EWMA & price forward-fill |
| Stage 5 | SLSQP Constrained Ensembling | Ensemble | 0.6910 | Multi-depth tree diversification |
| Stage 7 | Optuna Pearson-r Tuned Blend | Ensemble | 0.6914 | Correlation-targeted objective |
| **Benchmark** | **Held-Out Test Hybrid (OOS)** | **Tri-Model** | **0.6887** | **Unbiased out-of-sample generalization** |

*Note: Validation correlations (0.0947 to 0.6914) are selection-biased due to iterative hyperparameter tuning on the 20% validation split. The held-out test correlation of 0.6887 represents our strict, unbiased out-of-sample benchmark.*

## 6.3 Dimensionality Reduction: Supervised Tree Selection vs PCA
In Stage 2, we tested whether unsupervised Principal Component Analysis (PCA) could effectively compress the 90-feature v2 space compared to supervised tree-based importance selection (Figure F4).

| Reduction Strategy | Components / Features | Pearson $r$ | Retention vs Full Space (0.6442) |
|---|---|---|---|
| Full Feature Matrix | 90 Features | 0.6442 | 100.0% (Baseline) |
| Tree-Selected Top-25 | 25 Features | 0.6435 | **99.89% (Optimal Parsimony)** |
| PCA (95% Variance Explained) | 44 Components | 0.5345 | 82.97% ($\Delta r = -0.1097$) |
| PCA (Equal Dimension) | 25 Components | 0.5184 | 80.47% ($\Delta r = -0.1258$) |

Supervised selection identifies the 25 most informative volatility and spread features, preserving 99.89% of predictive power while eliminating 72% of the input dimensionality. Conversely, unsupervised PCA suffers severe degradation ($\Delta r \approx -0.11$). Because PCA maximizes variance along directions of maximum data dispersion, trending non-stationary price levels dominate the leading principal components, while stationary, low-variance volatility signals are relegated to discarded minor components.

## 6.4 Econometric Benchmark: Corsi HAR-RV and Regularized Ridge
Stage 8 implemented Corsi's (2009) Heterogeneous Autoregressive model alongside regularized multi-asset extensions, evaluated on the 3-way partition (Figure F5):

| Econometric Specification | Features | Val $r$ | Test $r$ (OOS) | Test RMSE | Test QLIKE |
|---|---|---|---|---|---|
| Persistence Baseline ($\text{RV}_{10}$) | 1 Feature | 0.6996 | 0.5441 | $6.200 \times 10^{-5}$ | 1,519,149.5* |
| Canonical HAR-RV (Corsi OLS) | 4 Regressors | 0.7740 | 0.6710 | $5.002 \times 10^{-5}$ | 0.7218 |
| Extended Multi-Scale HAR-RV (ETH) | 7 Regressors | 0.7820 | 0.6820 | $4.879 \times 10^{-5}$ | 0.7133 |
| Cross-Asset HAR-RV-X (ETH + BTC) | 12 Regressors | 0.7847 | 0.6843 | $4.864 \times 10^{-5}$ | 0.7123 |
| Regularized Ridge (30 Vol Features) | 30 Features | 0.7866 | **0.6860** | **$4.841 \times 10^{-5}$** | **0.7111** |

*\*Note: Persistence QLIKE is inflated by numerical division by near-zero unscaled persistence values. Regression coefficients in canonical OLS models exhibit inflated classical $t$-statistics ($t > 200$) due to overlapping rolling labels; Newey-West HAC adjusted standard errors (lag $L=10$) confirm that all multi-horizon coefficients remain statistically significant at $p < 0.0001$.*

**Key Econometric Insight:** The regularized multi-asset Ridge model on 30 volatility features achieves an out-of-sample correlation of $r = 0.6860$, outperforming canonical OLS HAR ($0.6710$) and beating standalone XGBoost ($0.6749$). Notably, when unscaled non-volatility LOB depth features were added to Ridge without non-linear filtering (`Microstructure HAR-RV (RidgeCV Vol+LOB)` in Stage 8), out-of-sample performance collapsed to $r \approx 0.02$ and test RMSE exploded to $0.00152$ (~30 times the target scale). This collapse was an out-of-sample numerical blow-up driven by extreme test-slice feature values rather than an inherent failure of linear modeling.

## 6.5 Ensembling Dynamics and the Forecast Combination Puzzle
In Stage 6, we evaluated stacked generalization by training meta-learners on validation prediction matrices. The prediction correlation heatmap (Figure F7) reveals extreme collinearity among base tree models:

| Correlation ($r$) | Persistence | Ridge (30 Vol) | LightGBM | XGBoost | Tree Blend | Hybrid |
|---|---|---|---|---|---|---|
| Persistence | 1.000 | 0.782 | 0.793 | 0.787 | 0.792 | 0.814 |
| Ridge (30 Vol) | 0.782 | 1.000 | 0.942 | 0.951 | 0.950 | 0.985 |
| LightGBM | 0.793 | 0.942 | 1.000 | **0.965** | 0.989 | 0.981 |
| XGBoost | 0.787 | 0.951 | **0.965** | 1.000 | 0.993 | 0.985 |
| Tree Blend | 0.792 | 0.950 | 0.989 | 0.993 | 1.000 | 0.986 |
| Hybrid | 0.814 | 0.985 | 0.981 | 0.985 | 0.986 | 1.000 |

Because LightGBM and XGBoost predictions share a massive correlation of $r = 0.965$, an XGBoost stacking meta-learner overfits validation residuals, collapsing to $r = 0.6615$ on the 80/20 validation split (compared to $0.6910$ for linear SLSQP blending). This provides a textbook demonstration of the **Bates-Granger (1969) Forecast Combination Puzzle**: simple linear combinations with fixed weights consistently beat complex non-linear meta-learners in the presence of collinear base forecasters.

## 6.6 Authoritative 3-Way Chronological Benchmark
The definitive out-of-sample benchmark (70% train, 15% validation, 15% held-out test) evaluates 12 model specifications scored on the 90,892-tick test block (Figure F6):

| Rank | Model / Strategy Architecture | Validation $r$ | Validation RMSE | Test $r$ (OOS) | Test RMSE (OOS) |
|---|---|---|---|---|---|
| 1 | **Econometric-Tree Hybrid (1/3 Ridge, 1/3 LGB, 1/3 XGB)** | **0.7841** | **$5.840 \times 10^{-5}$** | **0.6887** | **$4.797 \times 10^{-5}$** |
| 2 | Regularized RidgeCV (30 Vol Features) | 0.7866 | $5.627 \times 10^{-5}$ | 0.6860 | $4.841 \times 10^{-5}$ |
| 3 | Equal-Weight Tree Blend (50% LGBM + 50% XGB) | 0.7583 | $6.161 \times 10^{-5}$ | 0.6828 | $4.899 \times 10^{-5}$ |
| 4 | XGBoost Deep (Depth 6) | 0.7373 | $6.559 \times 10^{-5}$ | 0.6764 | $4.988 \times 10^{-5}$ |
| 5 | XGBoost Tuned (Stage 7) | 0.7376 | $6.555 \times 10^{-5}$ | 0.6752 | $4.990 \times 10^{-5}$ |
| 6 | XGBoost Conservative (Depth 4) | 0.7360 | $6.557 \times 10^{-5}$ | 0.6749 | $4.981 \times 10^{-5}$ |
| 7 | LightGBM Tuned (Stage 7) | 0.7604 | $6.033 \times 10^{-5}$ | 0.6737 | $5.172 \times 10^{-5}$ |
| 8 | Stage 5 SLSQP Tree Blend (Weights Tuned on Val) | 0.7636 | $5.950 \times 10^{-5}$ | 0.6711 | $5.299 \times 10^{-5}$ |
| 9 | LightGBM Conservative (Depth 4) | 0.7636 | $5.942 \times 10^{-5}$ | 0.6695 | $5.343 \times 10^{-5}$ |
| 10 | LightGBM Deep (Depth 6) | 0.7543 | $6.065 \times 10^{-5}$ | 0.6459 | $5.572 \times 10^{-5}$ |
| 11 | Baseline 1: Persistence ($\text{RV}_{10}$) | 0.6996 | $6.998 \times 10^{-5}$ | 0.5441 | $6.200 \times 10^{-5}$ |
| 12 | Baseline 2: Univariate OLS ($\text{RV}_{10}$) | 0.6996 | $7.080 \times 10^{-5}$ | 0.5441 | $6.049 \times 10^{-5}$ |

### Key Benchmark Takeaways
1. **Hybrid Outperforms All Constituents:** The Econometric-Tree Hybrid achieves the lowest test RMSE ($4.797 \times 10^{-5}$) and highest test correlation ($r = 0.6887$).
2. **Equal Weights Beat Optimized Weights:** The 50/50 Equal-Weight Tree Blend ($r = 0.6828$) strictly outperforms the validation-optimized SLSQP blend ($r = 0.6711$). SLSQP overfits validation idiosyncrasies, whereas equal weighting provides robust shrinkage against parameter uncertainty.
3. **Universal Validation Degradation:** All 12 models drop from validation to test (mean $\Delta r \approx -0.09$, persistence drops from $0.700$ to $0.544$). As shown in Figure F8, this reflects a macro market volatility regime shift during the held-out period.

## 6.7 Stationarity Ablation Experiment
To verify whether raw non-stationary price levels contaminate gradient boosted trees, we executed a stationarity ablation in `src/verify_data_integrity.py`:
- XGBoost Conservative trained on **all 135 features** (including 33 raw price levels): **Test $r = 0.6749$**
- XGBoost Conservative trained on **102 stationary features only** (raw prices dropped): **Test $r = 0.6750$**

The difference ($\Delta r = +0.0001$) shows that raw price levels have no measurable effect on forward volatility prediction. Constrained tree depth and column subsampling allow trees to ignore trending price levels in favor of stationary volatility ratios.

## 6.8 Out-of-Sample Temporal Stability (Rolling $r$)
Figure F8 tracks rolling Pearson correlation (window $= 5,000$ ticks, step $= 1,000$ ticks) across the 90,892 test observations. While market volatility fluctuates across calm and turbulent sub-regimes, the top models (Econometric-Tree Hybrid, Ridge 30-Vol, and Equal Tree Blend) remain tightly clustered within $\approx 0.006$ of each other, maintaining competitive temporal stability and consistently outperforming the naive persistence baseline across the test trajectory.
