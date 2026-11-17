# Viva Revision Notes (Minimal & Direct)

---

## Stage 1: Raw Baselines & Overfitting Proof

### 1. The Core Question
Can standard ML models predict forward volatility directly from raw Level-2 order book snapshots without any feature engineering?

### 2. The Input Data
- File: `data/raw/train/ETH.csv`
- Raw Features (21): 
  - Level 1-5 Bid Prices (`bids[0].price` to `bids[4].price`)
  - Level 1-5 Bid Sizes (`bids[0].amount` to `bids[4].amount`)
  - Level 1-5 Ask Prices (`asks[0].price` to `asks[4].price`)
  - Level 1-5 Ask Sizes (`asks[0].amount` to `asks[4].amount`)
  - Level 1 Spread (`ask1 - bid1`)
- Target (`label`): 10-second forward rolling realized volatility of Ethereum.
- Split: Chronological 80% train (476,650 rows) and 20% validation (119,163 rows). No shuffle.

### 3. Models Tested
- Random Forest Regressor (50 trees, max depth 10)
- XGBoost Regressor (baseline & with early stopping)
- LightGBM Regressor (baseline & with early stopping)

### 4. Metrics Calculated
- **RMSE (Root Mean Squared Error)**: Standard scale error.
- **Pearson correlation ($r$)**: Measures directional co-movement (-1 to +1).

### 5. What Actually Happened (Results)
- Random Forest: $r = -0.0189$ (no correlation, worse than a random guess).
- LightGBM Baseline: $r = +0.0947$ (almost zero signal).
- XGBoost Baseline: $r = +0.0887$ (almost zero signal).
- **Early Stopping Discovery**: Both XGBoost and LightGBM stopped learning after just 5 to 6 trees.
- **Overfitting Proof**: When forced to train 1,000 trees without early stopping, RMSE jumped from $7.9 \times 10^{-5}$ up to $1.64 \times 10^{-4}$ (error doubled).

### 6. The 20-Second Viva Answer (Say this to the professor)
> "In Stage 1, we tested whether standard tree models could predict 10-second forward volatility using only raw 5-level order book depth. The result was an immediate failure: Pearson correlation was near zero ($r \approx 0.09$), and the models overfitted after just 5 boosting rounds. This proved that static raw order books have no direct volatility signal on their own, and that feature engineering with temporal memory was mandatory."


---

## Stage 2: Feature Engineering, Cross-Asset Feeds & PCA vs. Trees

### 1. The Core Question
Since raw price levels failed in Stage 1, can we engineer stationary financial signals with temporal memory? And when reducing dimensionality, does unsupervised PCA work, or do we need supervised feature selection?

### 2. What was Implemented?
1. **Cross-Asset Merging**:
   - Merged order books for Ethereum (ETH), Bitcoin (BTC), and Solana (SOL) by aligning matching timestamps.
2. **Stationary Financial Feature Engineering (90 features)**:
   - **Spreads**: `ask_price - bid_price` for levels 1 and 2.
   - **Log Returns**: $r_t = \ln(\text{mid}_t / \text{mid}_{t-1})$ (removes price level non-stationarity).
   - **Realized Volatility**: Rolling standard deviation of returns over 5, 15, and 30 ticks ($\text{std}(r_{t-k \dots t})$).
   - **Order Book Imbalance (OBI)**: For levels 1, 2, and 3:
     $$\text{OBI}_k = \frac{\text{BidSize}_k - \text{AskSize}_k}{\text{BidSize}_k + \text{AskSize}_k}$$
     (Values from -1 to +1; measures whether buyers or sellers are pressuring the order book).
3. **Dimensionality Reduction Benchmark**:
   - Compared **Supervised Tree Selection** (picking top 25 features by LightGBM split gain) vs. **Unsupervised PCA** (Principal Component Analysis).

### 3. What were the Results?
- **Raw Baselines (Stage 1)**: $r \approx 0.0947$
- **All 90 Engineered Features (LightGBM)**: $r = 0.6442$ (a 7x jump!)
- **Tree-Selected Top 25 Features**: $r = 0.6435$ (retains 99.9% of predictive signal with only 25 features).
- **PCA (95% Variance Explained, 44 components)**: $r = 0.5345$ (significant drop).
- **PCA (Fixed 25 Components)**: $r = 0.5184$ (dropped by over 0.12).

### 4. Why did PCA Fail? (Critical Examiner Question!)
- **PCA is unsupervised**: It looks only at $X$ and does not see the target label $y$.
- **PCA maximizes variance**: Features with large numbers (raw prices like ETH \$2,600, BTC \$65,000) have massive variance.
- Stationary signals (returns, realized volatility $\approx 10^{-5}$) have tiny numerical variance.
- Therefore, PCA components got dominated by trending raw prices (which carry no volatility signal) while compressing away the actual volatility memory.
- In contrast, tree feature importance is **supervised**—it measures what directly explains the target label.

### 5. The 20-Second Viva Answer (Say this to the professor)
> "In Stage 2, we engineered 90 stationary features across ETH, BTC, and SOL, including multi-horizon realized volatility and Order Book Imbalance (OBI). This boosted Pearson correlation from 0.09 to 0.644. When comparing dimensionality reduction, supervised tree selection retained 99.9% of performance with just 25 features (r = 0.6435), whereas unsupervised PCA collapsed to 0.5184 because PCA maximizes variance and prioritized non-stationary raw price levels over low-variance stationary volatility signals."


---

## Stage 3: Econometric Volatility Clustering & Optuna Tuning

### 1. The Core Question
Can econometric variance theory (Engle's 1982 ARCH model) improve feature engineering? And does Bayesian hyperparameter tuning (Optuna) boost performance beyond default tree settings?

### 2. What was Implemented?
1. **Econometric Features (expanded from 90 to 97 features)**:
   - **Squared Returns ($r_t^2$)**: Instantaneous variance shock (the core of Robert Engle's ARCH model).
   - **Absolute Returns ($|r_t|$)**: Captures shock magnitude without sign bias.
   - **10-tick Realized Volatility**: Explicitly calculated across ETH, BTC, and SOL.
   - **Data Hygiene Finding**: Attempting `fillna(0)` on missing SOL rows destroyed correlation ($0.64 \rightarrow 0.53$); strict `dropna()` was proven mathematically necessary.
2. **Optuna Bayesian Optimization (60 Trials)**:
   - Used Tree-structured Parzen Estimator (TPE).
   - Tuned: `learning_rate` (log scale: 0.005 to 0.15), `max_depth` (3 to 8), `num_leaves` (15 to 127), and L1/L2 regularization (`reg_alpha`, `reg_lambda`).
   - Objective: Directly maximized **Pearson correlation ($r$)** instead of standard MSE.

### 3. What were the Results?
- Default LightGBM on 97 features: $r = 0.6526$
- **Optuna-Tuned LightGBM**: **$r = 0.6558$** (RMSE = $7.6 \times 10^{-5}$)
- **Optuna-Tuned XGBoost (CUDA)**: **$r = 0.6440$** (RMSE = $7.9 \times 10^{-5}$)

### 4. Key Learnings & Nuances
1. **Feature Engineering vs. Hyperparameter Tuning**:
   - Feature engineering in Stage 2 gave a massive **$+0.55$** gain ($0.09 \rightarrow 0.64$).
   - Optuna tuning only added **$+0.011$** ($0.644 \rightarrow 0.656$).
   - *Lesson*: Tuning cannot create signal from nothing; features drive performance, tuning merely polishes parameters.
2. **Random Seed Pinning**:
   - Early exploratory Optuna trials showed $\pm 0.003$ run-to-run variation.
   - We explicitly pinned `seed=42` on `TPESampler` to enforce strict scientific reproducibility.

### 5. The 20-Second Viva Answer (Say this to the professor)
> "In Stage 3, we grounded our pipeline in econometric theory by adding Robert Engle's ARCH variance proxies—specifically squared returns ($r_t^2$) and absolute returns ($|r_t|$)—expanding to 97 features. We then ran a 60-trial Optuna Bayesian search directly maximizing Pearson correlation. LightGBM reached r = 0.6558. A key insight was that tuning only added +0.011 over Stage 2, proving that domain-driven feature engineering dominates hyperparameter adjustments."


---

## Stage 4: Multi-Scale Continuous Pipeline & EWMA Volatility

### 1. The Core Question
In Stages 2 and 3, `dropna()` dropped rows when computing rolling windows, breaking continuous time. Can we build a continuous, unbroken tick pipeline, and does continuous exponential memory (EWMA) outperform simple rolling windows?

### 2. What was Implemented?
1. **Continuous Pipeline Reconstruction**:
   - Instead of discarding rows, prices were cleanly forward-filled and rolling windows used `min_periods=2`.
   - Scaled to **605,938 continuous ticks** without artificial time jumps.
2. **Multi-Scale Feature Set (135 features total: 33 raw, 102 stationary)**:
   - **EWMA Volatility (RiskMetrics Formulation)**:
     $$\text{EWMA Vol}_\alpha = \sqrt{\text{EWM}(r_t^2, \text{span}=\alpha)}$$
     Across spans of 10, 30, and 60 ticks. Puts high exponential weight on recent ticks and smoothly decays past noise.
   - **Normalized Quote Range Proxy**:
     $$\text{Range}_w = \frac{\max(\text{ask\_price5}) - \min(\text{bid\_price5})}{\text{mid\_price}}$$
     Measures the total structural width of the 5-level order book.
   - **Cross-Asset Interaction Signals**:
     - Price ratios (`mid_ETH / mid_BTC`, `mid_ETH / mid_SOL`).
     - Return differentials (`ret_ETH - ret_BTC`).
3. **Dual Model Evaluation (LightGBM vs. XGBoost on GPU)**:
   - Both trained on the full 135-feature continuous space.

### 3. What were the Results?
- Stage 1 Raw Baseline: $r = 0.0947$
- Stage 2 Feature Engineering: $r = 0.6441$
- Stage 3 Volatility Clustering: $r = 0.6558$
- **Stage 4 LightGBM**: **$r = 0.6811$** (RMSE = $5.7 \times 10^{-5}$)
- **Stage 4 XGBoost (CUDA)**: **$r = 0.6851$** (RMSE = $5.7 \times 10^{-5}$)

### 4. The Critical Discovery (Sets up Stages 5 & 6)
- **High Prediction Collinearity**:
  - LightGBM and XGBoost predictions had a correlation of **$r > 0.96$**.
  - Because both algorithms are decision trees splitting on the same features, they learn nearly identical error patterns.
  - This prompted the question: *Can ensembling squeeze any further performance out of such collinear models, or will it overfit?*

### 5. The 20-Second Viva Answer (Say this to the professor)
> "In Stage 4, we eliminated sample fragmentation by constructing an unbroken 605,938-tick continuous pipeline. We introduced 135 multi-scale features, notably RiskMetrics EWMA volatility filters across 10, 30, and 60 ticks and normalized 5-level quote range proxies. This pushed Pearson correlation past the 0.68 mark (r = 0.6851 on XGBoost). Crucially, we observed that LightGBM and XGBoost predictions shared a collinearity of r > 0.96, motivating our investigation into ensembling in Stages 5 and 6."


---

## Stages 5 & 6: Ensembles, Stacking & The Forecast Combination Puzzle

### 1. The Core Question
Stage 4 revealed that LightGBM and XGBoost predictions were 96% collinear. Can we combine multiple tree architectures? Does numerical weight optimization (SLSQP in Stage 5) or non-linear Stacking (Stage 6) improve out-of-sample forecasting?

### 2. What was Implemented?
1. **The 5-Model Candidate Suite**:
   Trained 5 diverse tree models with varying inductive biases:
   - LightGBM Conservative (Depth 4, 15 leaves, high regularization)
   - LightGBM Deep (Depth 6, 40 leaves)
   - LightGBM Huber (uses Huber loss to down-weight heavy-tailed volatility outliers)
   - XGBoost Conservative (Depth 4, CUDA)
   - XGBoost Deep (Depth 6, CUDA)
2. **Stage 5: Linear Blending (SLSQP Optimization)**:
   - Used Sequential Least Squares Programming (SLSQP) to find weights $\mathbf{w}$ maximizing Pearson $r$.
   - Bounded to the unit simplex: $\sum w_i = 1$ and $0 \le w_i \le 1$ (no negative extrapolation).
3. **Stage 6: Stacking with Out-of-Fold Meta-Learners**:
   - Used chronological 3-Fold `TimeSeriesSplit` (no future leakage) to generate out-of-fold predictions.
   - Trained two meta-learners on top of base model predictions:
     * **Ridge Meta-Learner** (regularized linear combination)
     * **XGBoost Meta-Learner** (non-linear tree combination)

### 3. What were the Results?
- **Stage 5 SLSQP Linear Blend**:
  - Validation: $r = 0.6910$ (Weights: 19.9% LGBM-Conservative + 20.4% LGBM-Deep + 59.7% XGB-Conservative).
  - Out-of-Sample Test: Dropped to **$r = 0.6711$** (Validation-tuned weights overfitted!).
- **Stage 6 Stacking Meta-Learners**:
  - Ridge Meta-Learner: $r = 0.6895$ (Linear combination remained stable).
  - XGBoost Meta-Learner: **$r = 0.6615$** (Non-linear stacking failed badly!).

### 4. Why did Non-Linear Stacking and Tuned Weights Fail? (The Forecast Combination Puzzle)
- **High Collinearity**: Base trees already shared $r > 0.96$ prediction correlation.
- **The Forecast Combination Puzzle (Bates & Granger, 1969; Timmermann, 2006)**:
  - When base forecasts are highly collinear, optimizing weights or fitting a complex non-linear meta-learner overfits sample-specific covariance in the validation set.
  - A complex non-linear meta-learner (XGBoost) tries to find intricate non-linear decision boundaries between predictions that are already nearly identical, capturing pure noise.
  - Simple equal-weighting or regularized linear combination eliminates parameter estimation error and generalizes far better out-of-sample.

### 5. The 20-Second Viva Answer (Say this to the professor)
> "In Stages 5 and 6, we investigated whether ensembling could extract further gains from our collinear tree models. In Stage 5, SLSQP linear blending achieved r = 0.6910 on validation, but degraded to 0.6711 out-of-sample. In Stage 6, non-linear XGBoost stacking failed significantly (r = 0.6615) compared to linear Ridge stacking (r = 0.6895). This provided clear empirical proof of the classic Forecast Combination Puzzle (Bates & Granger, 1969): when base models share extreme collinearity (r > 0.96), complex meta-learners overfit validation noise, and simple constrained linear combinations dominate."


---

## Stages 7 & 8: Econometric HAR-RV & The Final Winning Hybrid

### 1. Stage 7: Direct Optuna Ensemble Optimization
- **What was done**: Instead of SLSQP, we used Optuna (Bayesian TPE) to search for weights that directly maximized Pearson correlation ($r$) across tuned LightGBM, tuned XGBoost, and deep LightGBM.
- **Result**: Validation reached $r = 0.6914$ (Blend: 20.7% LGBM-Tuned + 58.3% XGB-Tuned + 21.1% LGBM-Deep).
- **Takeaway**: Like SLSQP in Stage 5, fine-tuning weights directly on validation gave a tiny validation boost ($+0.0004$) that failed to outperform simple equal-weighting out-of-sample.

---

### 2. Stage 8: Econometric HAR-RV & Multi-Asset RidgeCV

#### Why did we implement Corsi (2009) HAR-RV?
In academic finance, machine learning models are judged against **Corsi's (2009) Heterogeneous Autoregressive model of Realized Volatility (HAR-RV)**.
- Financial volatility has a **cascade structure** across different market participants:
  1. High-frequency traders (Ultra-fast: 5 ticks)
  2. Institutional execution algorithms (Medium: 15 ticks)
  3. Macro rebalancers & trend followers (Slow: 60 ticks)
- **Canonical Corsi (2009) HAR-RV**:
  $$RV_{t+10} = \beta_0 + \beta_1 RV_t^{(5)} + \beta_2 RV_t^{(15)} + \beta_3 RV_t^{(60)} + \beta_4 r_t^2 + \epsilon_t$$
  - Estimated with closed-form Ordinary Least Squares (OLS) and Newey-West HAC standard errors.
  - Runs in **under 3 seconds**.
  - **Result**: Out-of-sample Test **$r = 0.6710$**, RMSE = $5.002 \times 10^{-5}$.
- **Regularized Multi-Asset RidgeCV (30 Volatility Features)**:
  - We extended Corsi's multi-scale philosophy across 30 volatility and EWMA signals for ETH, BTC, and SOL.
  - Used L2 regularization (`RidgeCV`) across $\alpha \in [10^{-4}, 10^{4}]$.
  - **Result**: Out-of-sample Test **$r = 0.6860$**, RMSE = $4.841 \times 10^{-5}$!
  - **The Shock Finding**: This simple regularized linear model **beat standalone XGBoost (0.6749) and LightGBM (0.6695)!**
  - *Why?* Financial volatility has long-memory linear persistence. Linear models extrapolate smoothly, whereas decision trees slice continuous space into rigid step functions that can stumble during regime transitions.

---

### 3. The Grand Finale: Rigorous 3-Way Benchmark & The Winning Hybrid

To eliminate all selection bias, we evaluated all models on a strictly held-out chronological test set (**90,892 ticks**, the final 15% of the data):

| Model / Architecture | Validation $r$ | Test $r$ (Out-of-Sample) | Test RMSE | Why it Performed This Way |
| :--- | :---: | :---: | :---: | :--- |
| **Persistence Baseline** | 0.6996 | 0.5441 | $6.200 \times 10^{-5}$ | Non-parametric floor; dropped due to macro regime shift |
| **Canonical Corsi HAR-RV (OLS)** | 0.7740 | 0.6710 | $5.002 \times 10^{-5}$ | 4 features; trains in 2s; strong econometric baseline |
| **LightGBM Conservative (Depth 4)** | 0.7636 | 0.6695 | $5.343 \times 10^{-5}$ | Non-linear order book thresholding |
| **XGBoost Conservative (Depth 4)** | 0.7360 | 0.6749 | $4.981 \times 10^{-5}$ | Level-wise symmetrical splits |
| **Equal-Weight Tree Blend (50/50)** | 0.7583 | 0.6828 | $4.899 \times 10^{-5}$ | Simple average beat SLSQP validation-tuned weights (0.6711) |
| **Regularized Ridge (30 Vol)** | 0.7866 | 0.6860 | $4.841 \times 10^{-5}$ | Smooth linear long memory across ETH, BTC, and SOL |
| **Econometric-Tree Hybrid**<br>*(1/3 Ridge + 1/3 LGBM + 1/3 XGB)* | **0.7841** | **0.6887** | **$4.797 \times 10^{-5}$** | **PEAK WINNER: Combines linear memory + non-linear depth** |

#### Why did the Econometric-Tree Hybrid win?
1. **Ridge** provides smooth, un-partitioned linear volatility memory across all 3 crypto assets.
2. **LightGBM & XGBoost** provide non-linear detection of Order Book Imbalance (OBI) replenishment and spread widening.
3. **Equal weighting ($\frac{1}{3}$ each)** avoids parameter estimation error (Forecast Combination Puzzle).

---

### 4. The 20-Second Viva Answer (Say this to the professor)
> "In Stage 8, we implemented Corsi's (2009) canonical HAR-RV model, achieving an out-of-sample r of 0.6710 in under 3 seconds. When regularized via RidgeCV across 30 multi-asset volatility signals, it achieved r = 0.6860, outperforming standalone gradient boosted trees. Finally, our rigorous 3-way held-out benchmark revealed our top overall model: an Econometric-Tree Hybrid combining equal thirds of Ridge, LightGBM, and XGBoost. It achieved our peak out-of-sample correlation of r = 0.6887 by synthesizing linear volatility memory with non-linear order book depth thresholds."
