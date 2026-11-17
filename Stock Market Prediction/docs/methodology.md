# Empirical Methodology

## 1. Temporal Integrity & Validation Design
Because high-frequency financial time series exhibit strong temporal autocorrelation and non-stationarity, standard randomized K-Fold cross validation causes catastrophic lookahead data leakage. All evaluations use strict chronological partitioning (`shuffle=False`):
- **Exploratory Tuning Partition (80/20):** 484,750 train / 121,188 validation observations.
- **Rigorous 3-Way Benchmark Partition (70/15/15):** 424,156 train / 90,890 validation / 90,892 untouched test observations.

## 2. Evaluation Metrics
In high-frequency markets where forward volatility magnitude is minuscule ($\sim 10^{-5}$), standard scale metrics like MSE or RMSE can be deceptively small even for naive mean-predicting baselines.

We evaluate models across three complementary metrics:
1. **Pearson Information Coefficient (IC / $r$):**
   $$r = \frac{\sum (y_i - \bar{y})(\hat{y}_i - \bar{\hat{y}})}{\sqrt{\sum (y_i - \bar{y})^2 \sum (\hat{y}_i - \bar{\hat{y}})^2}}$$
   Measures true directional ranking and co-movement with forward volatility expansion/contraction regimes.
2. **Root Mean Squared Error (RMSE):**
   $$\text{RMSE} = \sqrt{\frac{1}{N}\sum_{i=1}^N (y_i - \hat{y}_i)^2}$$
3. **Quasi-Likelihood Robust Loss (QLIKE, Patton 2011):**
   $$\text{QLIKE}(y, \hat{y}) = \frac{y}{\hat{y}} - \ln\left(\frac{y}{\hat{y}}\right) - 1$$
   Patton (2011) proved that QLIKE is asymptotically robust to imperfect volatility proxies.

## 3. Early Stopping & Microstructural Noise Overfitting
Because target volatility magnitude is small ($\sim 10^{-5}$), decision tree ensembles face immediate risk of fitting idiosyncratic high-frequency micro-ticks.

Controlled experiments in `stage1_overfitting_proof.py` demonstrated that unconstrained boosting (1,000 trees) degrades validation RMSE from $0.000079 \rightarrow 0.000132$ (XGBoost) and $0.000164$ (LightGBM). Early stopping (stopping rounds 15–50 with learning rate $\eta = 0.03 - 0.05$) is strictly enforced across all tree models.

## 4. Econometric Benchmark: Corsi (2009) HAR-RV
To provide an institutional financial benchmark, we implement Corsi's Heterogeneous Autoregressive Realized Volatility model based on the Heterogeneous Market Hypothesis (Muller et al., 1997):
$$\sigma_{t+10s} = \beta_0 + \beta_s \sigma_t^{(5)} + \beta_m \sigma_t^{(15)} + \beta_l \sigma_t^{(60)} + \beta_{\text{shock}} r_t^2 + \beta_{\text{abs}} |r_t| + \epsilon$$

Because forward 10-step rolling realized volatility creates overlapping residuals, classical OLS standard errors underestimate parameter variance. We compute **Newey-West (1987) HAC standard errors** with Bartlett kernel ($L=10$). All volatility horizons remain statistically significant under HAC inference ($p < 0.0001$).

## 5. Regularized Linear Shrinkage (RidgeCV)
Rolling standard deviations at adjacent horizons (5, 10, 15, 30, 60 ticks) exhibit strong multicollinearity ($r > 0.90$). Standard OLS covariance matrices experience elevated condition numbers. $L_2$ Ridge regression shrinks parameters:
$$\hat{\beta}_{\text{ridge}} = (X^\top X + \alpha I)^{-1} X^\top y$$
Cross-validating $\alpha \in [10^{-4}, 10^{4}]$ on the 30-volatility matrix yields an out-of-sample Test $r = 0.6860$.

## 6. Ensemble Blending & The Forecast Combination Puzzle
We benchmark three combination schemes:
1. **Equal-Weight Blend (1/N):** Simple arithmetic mean of base learner predictions.
2. **Constrained SLSQP Optimization:** Sequential Least Squares Programming optimizing Pearson $r$ on validation data.
3. **TimeSeriesSplit Stacking:** Out-of-fold meta-learners (Ridge and XGBoost).

**Empirical Finding:** On the held-out test set, equal-weight blending ($r=0.6828$) outperformed validation-tuned SLSQP ($r=0.6711$). This illustrates the classical **Forecast Combination Puzzle** (Timmermann, 2006): optimizing combination weights on finite validation periods overfits regime noise. Combining linear HAR-style Ridge with tree models in the Econometric-Tree Hybrid achieves out-of-sample Test $r = 0.6887$.

## 7. Bayesian Optimization Protocol (Optuna TPE)
Hyperparameter tuning uses Tree-structured Parzen Estimators (TPE) with reproducible random seeds (`seed=42`):
- **Stage 2 Volatility Clustering:** 60 trials optimizing Pearson $r$.
- **Stage 3 Feature Selection:** 40 trials optimizing RMSE.
- **Stage 3 Advanced Tuning:** 30 trials each for XGBoost and LightGBM.
- **Stage 7 Direct Correlation Tuning:** 20 trials each for XGBoost and LightGBM directly maximizing Pearson $r$. Parameter search spaces: `max_depth` [3, 8], `num_leaves` [15, 128], `learning_rate` [0.01, 0.15], `colsample_bytree` [0.5, 1.0], `subsample` [0.5, 1.0].
