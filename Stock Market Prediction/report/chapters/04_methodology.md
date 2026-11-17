# 4. Methodology and Econometric Framework

## 4.1 Chronological Partitioning and Validation Protocol
In financial time series forecasting, standard machine learning cross-validation techniques—such as randomized $K$-fold cross-validation—are strictly invalid. Randomly partitioning rows destroys temporal autocorrelation, allows future market regimes to leak into the training filtration, and generates severely inflated, unreplicable performance metrics.

To ensure institutional validity and eliminate selection bias, our research enforces two distinct chronological partitioning frameworks:

```
Exploratory Protocol (Stages 1–7): 80% Train / 20% Validation (Chronological)
+-------------------------------------------------------+-------------------------+
|                  Train Set: 484,750 Ticks             | Validation: 121,188 Ticks|
+-------------------------------------------------------+-------------------------+

Rigorous 3-Way Protocol (Stage 8 & Authoritative Benchmark): 70% / 15% / 15%
+------------------------------------+------------------+-------------------------+
|        Train Set: 424,156 Ticks    | Val: 90,890 Ticks| Holdout Test: 90,892 Ticks|
|        (Model Parameter Fitting)   | (Early Stopping) | (Single Final Evaluation)|
+------------------------------------+------------------+-------------------------+
```

1. **Exploratory 80/20 Partition (Stages 1–7):** Utilized for initial feature exploration, Bayesian hyperparameter tuning, and preliminary model screening (484,750 training ticks and 121,188 validation ticks). Performance metrics reported under this split are explicitly tagged as **"selection-biased validation."**
2. **Authoritative 70/15/15 Three-Way Partition (Stage 8 & Rigorous Benchmark):**
   - **Training Set (70.0%, 424,156 ticks):** Used strictly for fitting base regression parameters and tree structures.
   - **Validation Set (15.0%, 90,890 ticks):** Used strictly for GBDT early stopping (50-round patience) and optimizing SLSQP ensemble blending weights.
   - **Held-Out Test Set (15.0%, 90,892 ticks):** An untouched out-of-sample block scored strictly once to produce unbiased, publication-grade test metrics.

## 4.2 Mathematical Formulation of the Feature Space
The complete engineered feature matrix comprises exactly **135 continuous columns**, structured into three complementary tiers:

### Tier 1: Raw Level-2 Order Book Signals (63 Features)
For each asset $A \in \{\text{ETH}, \text{BTC}, \text{SOL}\}$, 21 raw columns are ingested:
$$\mathbf{x}_{t, \text{raw}}^{(A)} = \left[ P_{t, \text{mid}}^{(A)}, \left\{ P_{t, \text{bid}, k}^{(A)}, V_{t, \text{bid}, k}^{(A)}, P_{t, \text{ask}, k}^{(A)}, V_{t, \text{ask}, k}^{(A)} \right\}_{k=1}^5 \right]$$

### Tier 2: Engineered Microstructure & Volatility Signals (63 Features, 21 per Asset)
1. **Instantaneous Return Signals:**
   $$r_t = \ln\left( \frac{P_t^{\text{mid}}}{P_{t-1}^{\text{mid}}} \right), \quad r_t^2 \quad (\text{ARCH proxy}), \quad |r_t| \quad (\text{Absolute magnitude})$$
2. **Multi-Horizon Realized Volatility ($\text{RV}_w$):**
   $$\text{RV}_{t, w} = \sqrt{\frac{1}{w-1} \sum_{i=0}^{w-1} \left( r_{t-i} - \bar{r}_w \right)^2}, \quad w \in \{5, 10, 15, 30, 60\} \text{ ticks}$$
3. **RiskMetrics Continuous EWMA Volatility:**
   $$\sigma_{t, \lambda}^2 = (1 - \lambda) r_t^2 + \lambda \sigma_{t-1, \lambda}^2, \quad \text{Spans } S \in \{10, 30, 60\} \implies \lambda \in \{0.8182, 0.9355, 0.9672\}$$
4. **Order Book Depth Imbalance ($\text{OBI}_k$):**
   $$\text{OBI}_{t, k} = \frac{V_{t, k}^{\text{bid}} - V_{t, k}^{\text{ask}}}{V_{t, k}^{\text{bid}} + V_{t, k}^{\text{ask}} + 10^{-8}}, \quad k \in \{1, 2, 3\}$$
5. **Bid-Ask Spreads Across Depth ($\text{Spread}_k$):**
   $$\text{Spread}_{t, k} = P_{t, k}^{\text{ask}} - P_{t, k}^{\text{bid}}, \quad k \in \{1, 2, 3, 4, 5\}$$
6. **Normalized Book Range Proxies:**
   $$\text{Range}_{t, w} = \frac{\max_{i \in [0, w]} P_{t-i}^{\text{ask5}} - \min_{i \in [0, w]} P_{t-i}^{\text{bid5}}}{P_t^{\text{mid}} + 10^{-8}}, \quad w \in \{15, 30\}$$

### Tier 3: Cross-Asset Relative & Autoregressive Indicators (9 Features)
- Autoregressive return lags: $r_{t-1}, r_{t-2}, r_{t-3}$
- Autoregressive realized volatility lags: $\text{RV}_{t-1, 10}, \text{RV}_{t-2, 10}, \text{RV}_{t-3, 10}$
- Cross-asset price ratios: $P_t^{\text{ETH}} / P_t^{\text{BTC}}$ and $P_t^{\text{ETH}} / P_t^{\text{SOL}}$
- Cross-asset return spread: $r_t^{\text{ETH}} - r_t^{\text{BTC}}$

| Feature Group | Count | Mathematical Rationale |
|---|---|---|
| Raw Level-2 Depth (3 assets $\times$ 21) | 63 | Ingests foundational queue depth and price levels |
| Engineered Volatility (3 assets $\times$ 11) | 33 | Captures volatility clustering, ARCH variance, and multi-scale memory |
| Microstructure Spreads & OBI (3 assets $\times$ 10) | 30 | Quantifies instantaneous order flow pressure and quoting wideness |
| Cross-Asset & Autoregressive Lags | 9 | Exploits lead-lag spillover from BTC and autoregressive persistence |
| **Total Feature Space** | **135** | **Comprehensive high-frequency continuous representation** |

## 4.3 Econometric Model Formulations
To establish robust quantitative foundations, we implement a hierarchical family of econometric time-series models:

### 1. Persistence Baseline
The simplest non-parametric benchmark assumes future volatility equals current realized volatility:
$$\hat{y}_{t}^{\text{persist}} = \text{RV}_{t, 10}$$

### 2. Univariate Ordinary Least Squares (OLS)
Calibrates affine scaling to the persistence measure:
$$\hat{y}_{t}^{\text{OLS}} = \hat{\alpha} + \hat{\beta} \text{RV}_{t, 10}$$

### 3. Canonical HAR-RV (Corsi, 2009)
Adapts Corsi's three-component cascade using tick-frequency horizons ($w_1 = 5$ ticks, $w_2 = 15$ ticks, $w_3 = 60$ ticks):
$$y_t = \beta_0 + \beta_1 \text{RV}_{t, 5}^{(\text{ETH})} + \beta_2 \text{RV}_{t, 15}^{(\text{ETH})} + \beta_3 \text{RV}_{t, 60}^{(\text{ETH})} + \epsilon_t$$

### 4. Regularized Multi-Asset HAR-Style Ridge (30 Vol Features)
Extends the linear econometric framework across all 30 volatility features spanning $\text{ETH}$, $\text{BTC}$, and $\text{SOL}$ (realized volatilities, EWMA filters, squared returns, and absolute returns). To avoid multicollinearity in high-dimensional linear regression, we standardize features and optimize the $\ell_2$ penalty parameter $\alpha$ via generalized cross-validation across 15 logarithmic steps:
$$\hat{\boldsymbol{\beta}}_{\text{Ridge}} = \arg\min_{\boldsymbol{\beta}} \left\{ \|\mathbf{y} - \mathbf{X}_{\text{vol}} \boldsymbol{\beta}\|_2^2 + \alpha \|\boldsymbol{\beta}\|_2^2 \right\}, \quad \alpha \in [10^{-4}, 10^{4}]$$

## 4.4 Non-Linear Gradient Boosted Decision Trees
To capture non-linear thresholds and interaction effects between order book imbalances and volatility regimes, we deploy LightGBM and XGBoost under two distinct structural inductive biases:
1. **Conservative Models (Depth 4):** Strictly constrained tree depth (max depth $= 4$, num leaves $= 15$) with aggressive column subsampling (`colsample_bytree` $= 0.60 - 0.70$) and row subsampling (`subsample` $= 0.80$). This architecture enforces shallow decision paths that resist noise overfitting.
2. **Deep Models (Depth 6):** Moderately expanded capacity (max depth $= 6$, num leaves $= 40$) with learning rate $\eta = 0.03$, capturing higher-order cross-asset interaction effects.

### Early Stopping Enforcement
In all tree training routines, early stopping is strictly enforced with a patience of 50 rounds evaluated exclusively on the validation partition:
```python
callbacks = [lgb.early_stopping(stopping_rounds=50, verbose=False)]
```
If validation loss fails to improve for 50 consecutive boosting rounds, training halts immediately, preventing the noise memorization documented in Stage 1.

## 4.5 Ensemble Architecture and Synthesis
We evaluate four ensembling strategies:
1. **Equal-Weight Tree Blend (50/50):** Simple arithmetic mean of LightGBM-Conservative and XGBoost-Conservative:
   $$\hat{y}_{t}^{\text{tree}} = 0.5 \hat{y}_{t}^{\text{LGBM-Cons}} + 0.5 \hat{y}_{t}^{\text{XGB-Cons}}$$
2. **Constrained SLSQP Blend:** Sequential Least Squares Programming optimization to find weights $\mathbf{w}$ that maximize validation Pearson correlation subject to $w_i \ge 0$ and $\sum w_i = 1$:
   $$\mathbf{w}^* = \arg\max_{\mathbf{w}} r\left( \mathbf{y}_{\text{val}}, \hat{\mathbf{Y}}_{\text{val}} \mathbf{w} \right)$$
3. **Stacked Generalization Meta-Learners:** Training non-linear (XGBoost) and linear (Ridge) meta-learners on base model validation prediction matrices.
4. **Econometric-Tree Hybrid:** Synthesizes linear econometric volatility memory with non-linear tree microstructure thresholding by taking equal thirds:
   $$\hat{y}_{t}^{\text{hybrid}} = \frac{1}{3} \hat{y}_{t}^{\text{Ridge-30vol}} + \frac{1}{3} \hat{y}_{t}^{\text{LGBM-Cons}} + \frac{1}{3} \hat{y}_{t}^{\text{XGB-Cons}}$$

## 4.6 Evaluation Metrics and Loss Rationale
Models are evaluated across three complementary quantitative metrics:
1. **Pearson Correlation Coefficient ($r$):** The primary optimization and selection metric:
   $$r(\mathbf{y}, \hat{\mathbf{y}}) = \frac{\sum_{i=1}^N (y_i - \bar{y})(\hat{y}_i - \bar{\hat{y}})}{\sqrt{\sum_{i=1}^N (y_i - \bar{y})^2 \sum_{i=1}^N (\hat{y}_i - \bar{\hat{y}})^2}}$$
   *Rationale:* In institutional volatility trading, the scale of volatility is tiny (magnitude $\sim 10^{-5}$). Traders require directional co-movement and rank correlation to dynamically adjust quotes, making Pearson $r$ more informative than raw mean squared error.
2. **Root Mean Squared Error (RMSE):**
   $$\text{RMSE} = \sqrt{\frac{1}{N} \sum_{i=1}^N (y_i - \hat{y}_i)^2}$$
3. **Patton (2011) Robust QLIKE Loss:**
   $$\text{QLIKE} = \frac{1}{N} \sum_{i=1}^N \left( \frac{y_i}{\hat{y}_i} - \ln\left(\frac{y_i}{\hat{y}_i}\right) - 1 \right)$$
