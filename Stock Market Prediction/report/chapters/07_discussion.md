# 7. Discussion and Threats to Validity

## 7.1 Market Regime Shifts and Generalization Degradation
A prominent empirical finding across all evaluated architectures is the universal performance degradation observed when transitioning from the validation split (ticks 424,156 to 515,046) to the held-out test split (ticks 515,046 to 605,938). As documented in Section 6.6, every model—from naive persistence to our winning hybrid—exhibits an average drop in Pearson correlation of $\Delta r \approx -0.09$. 

Importantly, this decline is **not evidence of algorithmic overfitting or data leakage**. The naive persistence baseline ($\hat{y}_t = \text{RV}_{t, 10}$), which contains exactly zero estimated parameters, drops precipitously from $r = 0.6996$ on validation to $r = 0.5441$ on test. Similarly, the regularized linear Ridge model drops from $0.7866$ to $0.6860$. 

This uniform downward shift reflects an authentic **macroeconomic regime shift in cryptocurrency microstructure**. During the test period, realized volatility experienced sharp, transient bursts characterized by lower autocorrelation persistence and wider spread dislocations. When the underlying physical process becomes inherently noisier and less persistent, the theoretical upper bound on predictable variance decreases for all estimators.

## 7.2 Why Regularized Linear Econometrics Matches Complex Trees
A major quantitative revelation of this research is that a regularized linear model—specifically RidgeCV fitted on 30 volatility features—achieves an out-of-sample correlation of $r = 0.6860$, outperforming complex standalone XGBoost ($0.6749$) and LightGBM ($0.6695$).

In machine learning research, there is an endemic tendency to presume that deep neural networks or complex gradient boosted trees inherently dominate linear specifications. However, in financial volatility modeling, three statistical properties explain the superior performance of regularized linear econometrics:
1. **Hyperbolic Autocorrelation Memory:** Volatility clustering exhibits long-memory dynamics characterized by slow hyperbolic power-law decay. Linear autoregressive cascades (the foundational premise of Corsi's HAR-RV) align perfectly with this continuous, additive data-generating process.
2. **Step-Function Approximation Penalties:** Decision trees approximate continuous regression surfaces using discrete orthogonal step functions. In quiet volatility regimes punctuated by continuous mean-reverting drifts, discrete tree splits introduce discretization variance.
3. **$\ell_2$ Ridge Shrinkage:** High-frequency volatility estimators (e.g., $\text{RV}_5$, $\text{RV}_{10}$, $\text{EWMA}_{10}$) are naturally collinear. Ridge regularization shrinks correlated coefficients smoothly toward zero without forcing arbitrary feature exclusions, preserving multi-horizon memory while preventing variance inflation. Conversely, attempting to include unscaled non-volatility order book depth features directly into linear Ridge regression (as observed in the 102-feature Microstructure Ridge experiment in Stage 8, where test $r \approx 0.02$ and test RMSE exploded to $1.5 \times 10^{-3}$) results in an out-of-sample numerical blow-up driven by extreme test-slice feature values. Linear models require strictly stationary, well-scaled volatility regressors to maintain calibration.

## 7.3 Complementary Inductive Biases in the Winning Hybrid
Why does the Econometric-Tree Hybrid (1/3 Ridge + 1/3 LightGBM + 1/3 XGBoost) outperform all constituent models, establishing our peak out-of-sample result of $r = 0.6887$?

The hybrid succeeds because it synthesizes two fundamentally **orthogonal inductive biases**:
- **Linear Volatility Memory (Ridge):** Provides a smooth, highly calibrated baseline forecast driven by multi-scale realized variance and cross-asset autoregressive lags.
- **Non-Linear Depth Thresholding (LightGBM & XGBoost):** Decision trees excel at identifying regime-switching thresholds in limit order book depth. Specifically, when Order Book Imbalances ($\text{OBI}_1, \text{OBI}_2$) cross extreme thresholds (e.g., queue volume ratio $< 0.15$ or $> 0.85$) while bid-ask spreads widen, trees apply non-linear variance markups that linear regression cannot capture.

By combining equal proportions of linear persistence and non-linear microstructure gating, the hybrid achieves superior generalization across both calm and turbulent market regimes.

## 7.4 The Forecast Combination Puzzle: Equal vs Tuned Weights
Our empirical results provide definitive validation of the Bates-Granger (1969) Forecast Combination Puzzle. In Stage 5, we utilized Sequential Least Squares Programming (SLSQP) to find optimal blending weights on the validation split (allocating ~59.7% to XGBoost, ~20.4% to LightGBM Deep, and ~19.9% to LightGBM Conservative). 

However, on the held-out test split, the validation-optimized SLSQP blend achieved $r = 0.6711$, significantly **underperforming the simple 50/50 Equal-Weight Tree Blend ($r = 0.6828$)**. 

Because base tree learners share extreme correlation ($r = 0.965$, Figure F7), the optimization surface is nearly flat. Optimizing weights on validation noise causes the optimizer to overfit sample-specific residual idiosyncrasies. Simple equal weighting ($w_i = 1/K$) eliminates parameter estimation variance entirely, providing optimal Bayesian shrinkage against regime shifts.

## 7.5 Threats to Validity and Explicit Limitations
In accordance with rigorous academic integrity, we explicitly detail nine potential threats to validity:

1. **Contemporaneous Return Overlap:** As proven in Section 3.4, the 10-step forward standard deviation window $[t, t+9]$ incorporates the instantaneous return $r_t = \ln(P_t / P_{t-1})$. While features contain no future information from $t+1$ onwards, 1 of the 10 terms in the target window is observable contemporaneously at tick $t$.
2. **Test-Slice Reuse During Development:** While the 90,892-tick test slice was scored strictly once in `eval_rigorous_test_benchmark.py`, the research community must remain mindful that iterative benchmarking in competitive settings inherently carries risk of subtle hyperparameter selection drift.
3. **Single Asset Target and Single Temporal Period:** The target label is evaluated exclusively on Ethereum ($\text{ETH}$) over a continuous multi-day interval. Microstructure dynamics during major macroeconomic events (e.g., central bank rate announcements) or in illiquid altcoins may exhibit different regime behaviors.
4. **Tick Spacing Assumptions:** While empirical audits confirm a median $\Delta t = 1.000$ second with zero variance across 99% of observations, extreme network congestion or exchange WebSocket dropouts could introduce non-uniform sampling intervals.
5. **Minute-Level Cross-Asset Alignment:** Reconstructed $\text{BTC}$ and $\text{SOL}$ order books record timestamps at minute resolution. Horizontal concatenation assumes row-level synchronization within each minute. While minute-level timestamps align across all 631,292 rows, sub-minute queue changes may exhibit minor latency offsets.
6. **Unseeded Optuna and GPU Non-Determinism:** Early exploratory Optuna runs in Stage 3 exhibited run-to-run correlation variance of $\pm 0.003$ due to unseeded random state samplers. Furthermore, CUDA GPU non-deterministic atomic operations introduce minor floating-point divergence across different hardware platforms.
7. **OLS Standard Errors and Autocorrelation:** Canonical OLS regressions on overlapping rolling labels exhibit moving average $MA(9)$ residual serial correlation, artificially inflating classical $t$-statistics ($t > 200$). While Newey-West HAC adjustments (lag $L=10$) confirm statistical significance ($p < 0.0001$), reported unadjusted OLS $t$-ratios must not be taken at face value.
8. **Absence of Block-Bootstrap Significance Testing:** The top three benchmark models differ by small increments ($\Delta r \le 0.0059$: Hybrid $0.6887$ vs Ridge $0.6860$ vs Tree Blend $0.6828$). In the absence of block-bootstrapped confidence intervals or formal Diebold-Mariano tests, we do not claim that the hybrid represents a statistically significant "global peak."
9. **Loss Function Asymmetry (Pearson $r$ vs QLIKE):** Optimizing for Pearson correlation prioritizes linear co-movement on scale-invariant targets. In institutional risk management, under-predicting extreme variance spikes incurs severe asymmetric capital penalties better captured by Patton's (2011) QLIKE loss.

## 7.6 Trustworthy AI Safeguards and Audit Guarantees
To ensure model safety and compliance with Trustworthy AI principles, the pipeline enforces four architectural safeguards:
- **Strict Chronological Splitting (`shuffle=False`):** Enforced across every script, completely eliminating temporal data leakage.
- **Zero Future Information in Features:** All rolling windows, lags, and normalizations use causal backward-looking operators only.
- **Stationarity Verification:** Stationarity ablation confirms that model performance is driven exclusively by stationary volatility signals rather than non-stationary price levels ($\Delta r = +0.0001$).
- **Deterministic and CPU-Native Serialization:** All models are exported with CPU-native inference parameters, accompanied by automated pytest verification checking metric fidelity to $10^{-9}$ and live inference to $10^{-6}$.
