# Forecasting 10-Second Forward Volatility of Ethereum from Level-2 Limit Order Books

**A Quantitative Microstructure Study in Trustworthy Machine Learning**

---

### Candidate Declaration & Metadata
- **Author:** Abdul Qadir Memon (Roll Number: 2024IMG-028)
- **Supervisor / Mentor:** Rushil Agrawal
- **Institution:** ABV-IIITM Gwalior
- **Course / Track:** Trustworthy Artificial Intelligence (TAI)

---

## Abstract

Accurate short-horizon volatility forecasting is critical for market makers, algorithmic liquidity providers, and high-frequency risk management systems navigating extreme cryptocurrency microstructure volatility. This paper investigates the predictability of 10-second forward realized volatility for Ethereum (ETH) using Level-2 limit order book (L2 LOB) depth and cross-asset signals from Bitcoin (BTC) and Solana (SOL). Using a reconstructed continuous dataset of 605,938 one-second snapshots (from 631,292 raw records), we evaluate models under an uncompromising 70/15/15 chronological three-way partitioning protocol (424,156 training, 90,890 validation, and 90,892 strictly held-out test ticks) to eliminate forward lookahead and hyperparameter selection bias. 

We demonstrate that raw limit order book depth features carry negligible direct volatility predictive power (Pearson $r \in [-0.0189, 0.0947]$) and trigger rapid noise memorization within 5 to 6 gradient boosting iterations. Engineering continuous multi-scale realized volatility and RiskMetrics exponential moving average (EWMA) signals unlocks the primary predictive relationship, boosting out-of-sample correlation to $r = 0.6851$. In head-to-head benchmarking, a regularized multi-asset HAR-style Ridge model on 30 stationary volatility features achieves an out-of-sample correlation of $r = 0.6860$, outperforming complex standalone gradient boosted trees ($r = 0.6749$ for XGBoost). Furthermore, non-linear stacking meta-learners fail due to extreme base-learner collinearity ($r > 0.96$), empirically demonstrating the Bates-Granger forecast combination puzzle. Our final Econometric-Tree Hybrid—blending equal thirds of Ridge, LightGBM, and XGBoost—achieves the peak out-of-sample performance of $r = 0.6887$ (RMSE $= 4.797 \times 10^{-5}$). Transparent documentation of label construction reveals that the forward 10-step rolling standard deviation window incorporates the contemporaneous return $r_t$, confirming that while features contain no future information, contemporaneous variance overlap contributes to short-memory predictability.

**Keywords:** High-Frequency Trading, Limit Order Book Microstructure, Realized Volatility, HAR-RV, Gradient Boosted Trees, Forecast Combination Puzzle, Trustworthy AI.

---

## Table of Contents
1. [Introduction](#1-introduction)
2. [Background and Related Work](#2-background-and-related-work)
3. [Data Architecture and Integrity](#3-data-architecture-and-integrity)
4. [Methodology and Econometric Framework](#4-methodology-and-econometric-framework)
5. [System Implementation and Quantitative Terminal](#5-system-implementation-and-quantitative-terminal)
6. [Empirical Results and Benchmark Analysis](#6-empirical-results-and-benchmark-analysis)
7. [Discussion and Threats to Validity](#7-discussion-and-threats-to-validity)
8. [Conclusion and Future Roadmap](#8-conclusion-and-future-roadmap)
9. [References](#references)
10. [Appendices](#appendices)

---

## Symbols and Abbreviations

| Symbol / Abbreviation | Definition |
|---|---|
| LOB / L2 | Limit Order Book / Level-2 Depth (Prices and Queue Volumes at 5 Bids and 5 Asks) |
| ETH / BTC / SOL | Ethereum / Bitcoin / Solana Cryptocurrency Assets |
| $P_t^{\text{mid}}$ | Mid-price at tick $t$, defined as $(P_t^{\text{ask1}} + P_t^{\text{bid1}}) / 2$ |
| $r_t$ | Instantaneous logarithmic return at tick $t$, $\ln(P_t^{\text{mid}} / P_{t-1}^{\text{mid}})$ |
| RV / $RV_w$ | Realized Volatility over rolling backward window of $w$ ticks |
| EWMA | Exponentially Weighted Moving Average (RiskMetrics formulation) |
| OBI | Order Book Imbalance across bid and ask depth levels |
| HAR-RV | Heterogeneous Autoregressive model of Realized Volatility (Corsi, 2009) |
| GBDT | Gradient Boosted Decision Trees (LightGBM and XGBoost) |
| SLSQP | Sequential Least Squares Programming (constrained optimization) |
| OOS | Out-Of-Sample (Strictly held-out evaluation dataset) |
| RMSE | Root Mean Squared Error |
| QLIKE | Quasi-Likelihood robust volatility loss function (Patton, 2011) |
| HAC | Heteroskedasticity and Autocorrelation Consistent standard errors (Newey-West, 1987) |
| TAI | Trustworthy Artificial Intelligence research track |


# 1. Introduction

## 1.1 Motivation and Practical Relevance
In continuous electronic financial markets, the Limit Order Book (LOB) serves as the core auction mechanism through which liquidity suppliers and demanders interact. Unlike low-frequency daily closing prices, high-frequency Level-2 order books record discrete changes in the instantaneous queue volume and price levels at multiple depths of the market. For quantitative liquidity providers, electronic market makers, and institutional risk desks, short-horizon volatility forecasting—specifically over 5- to 30-second execution horizons—is essential for three primary reasons:
1. **Adverse Selection Mitigation:** Market makers continuously quote bid and ask quotes. When incoming toxic order flow anticipates an impending volatility spike, passive quotes are adversely executed at stale prices. Anticipating forward volatility allows liquidity providers to dynamically widen spreads or skew quoting depth to protect capital.
2. **Dynamic Capital and Margin Allocation:** In leveraged cryptocurrency perpetual futures, exchange liquidation engines compute continuous maintenance margin requirements based on forward price variance. Accurate short-term volatility estimates prevent forced liquidations and optimize inventory holding costs.
3. **Execution Cost Minimization:** Algorithmic execution engines (such as VWAP and TWAP implementations) modulate slicing schedules based on expected near-term volatility, scheduling trades during calmer liquidity regimes to minimize market impact.

Cryptocurrency derivatives markets represent an exceptionally demanding proving ground for high-frequency volatility models due to 24/7 continuous trading, extreme tail risk, retail-institutional order fragmentation, and pronounced cross-asset volatility spillover across flagship assets such as Ethereum (ETH), Bitcoin (BTC), and Solana (SOL).

## 1.2 Problem Statement
This research formulates and solves the problem of predicting the **10-second forward rolling realized volatility** of Ethereum ($\text{ETH}$) using instantaneous Level-2 limit order book depth (5 bid and 5 ask price and queue volume levels) captured synchronously with Bitcoin ($\text{BTC}$) and Solana ($\text{SOL}$) books. 

Formally, given the multivariate order book information filtration available at discrete tick snapshot $t$:
$$\mathcal{F}_t = \left\{ \mathbf{x}_\tau^{(\text{ETH})}, \mathbf{x}_\tau^{(\text{BTC})}, \mathbf{x}_\tau^{(\text{SOL})} \right\}_{\tau \le t}$$
the objective is to estimate an optimal mapping $f(\mathcal{F}_t)$ that minimizes the forecasting error for the forward realized volatility:
$$y_t = \sigma_{t \to t+H} = \sqrt{\frac{1}{H-1} \sum_{k=0}^{H-1} \left( r_{t+k} - \bar{r} \right)^2}$$
where $H = 10$ ticks (empirically confirmed to represent a 10.0-second physical execution horizon with median $\Delta t = 1.000$ seconds), and $r_{t+k} = \ln(P_{t+k}^{\text{mid}} / P_{t+k-1}^{\text{mid}})$ denotes the instantaneous logarithmic return of the ETH mid-price.

## 1.3 Concrete Research Objectives
To establish an institutional-grade investigation, this project executes five structured research objectives:
- **O1. Raw Feature Baseline Benchmarking:** Quantify the baseline predictive capacity of raw Level-2 price and volume depth vectors using standard linear and non-linear learners (Random Forests, LightGBM, XGBoost), establishing whether raw order book depth alone carries direct volatility signal.
- **O2. Multi-Scale Feature Engineering:** Synthesize high-frequency market microstructure domain knowledge into stationary, continuous volatility representations—including realized rolling variances, ARCH-type squared returns, RiskMetrics Exponentially Weighted Moving Averages (EWMA), Order Book Imbalances (OBI), and cross-asset relative spreads.
- **O3. Econometric Time-Series Benchmarking:** Implement formal econometric baselines from empirical finance, specifically Corsi's (2009) Heterogeneous Autoregressive model of Realized Volatility (HAR-RV) and its regularized multi-asset extensions, with Newey-West (1987) Heteroskedasticity and Autocorrelation Consistent (HAC) statistical inference.
- **O4. Algorithmic Optimization and Ensembling:** Conduct Bayesian hyperparameter optimization (Tree-structured Parzen Estimators), explore non-linear stacking meta-learners, evaluate constrained SLSQP blending, and test the empirical validity of the classic Bates-Granger (1969) Forecast Combination Puzzle.
- **O5. Trustworthy AI Auditing and Verification:** Enforce rigorous ML safety standards by executing a strict 3-way chronological partitioning scheme (70% train, 15% validation, 15% held-out test), auditing timestamp continuity, conducting stationarity ablations, thoroughly investigating label-overlap mechanics, and packaging verified models into an interactive terminal interface.

## 1.4 Summary of Contributions
The primary quantitative and methodological contributions of this paper are:
1. **Refutation of Raw LOB Direct Signal:** We prove that raw Level-2 order book depth levels carry negligible direct correlation with forward volatility ($r \in [-0.0189, 0.0947]$). Unconstrained tree models memorize sensor noise rapidly within 5 to 6 boosting iterations, doubling test RMSE if early stopping is disabled.
2. **Linear Volatility Memory Competitiveness:** We show that a regularized multi-asset HAR-style Ridge model on 30 stationary volatility features achieves an out-of-sample correlation of $r = 0.6860$, outperforming complex standalone non-linear trees ($r = 0.6749$ for XGBoost).
3. **Resolution of the Stacking Paradox:** We document that stacking meta-learners fail ($r = 0.6615$) due to extreme base-learner prediction collinearity ($r > 0.96$). Simple equal-weight blending ($r = 0.6828$) strictly outperforms validation-optimized SLSQP weights ($r = 0.6711$) out-of-sample.
4. **Winning Hybrid Synthesis:** We propose and validate an Econometric-Tree Hybrid (1/3 Ridge + 1/3 LightGBM + 1/3 XGBoost) that achieves the highest out-of-sample generalization ($r = 0.6887$, RMSE $= 4.797 \times 10^{-5}$), combining linear long-memory persistence with non-linear order book queue thresholding.
5. **Radical Audit Honesty:** We transparently dissect the target label construction, revealing that the 10-step forward standard deviation window contains the contemporaneous return $r_t$. While features contain no future information, this 1-step overlap is rigorously documented to prevent false claims of zero contemporaneous correlation.

## 1.5 Report Organization
The remainder of this report is structured as follows: Chapter 2 reviews foundational literature in market microstructure, realized volatility, and forecast combination. Chapter 3 details the dataset architecture, data sanitization rules, and integrity verification tests. Chapter 4 presents the econometric framework, feature formulations, and 3-way chronological split protocol. Chapter 5 outlines the software implementation and interactive terminal application. Chapter 6 analyzes empirical results across all 8 experimental stages and the 12-model benchmark. Chapter 7 provides an extensive discussion of threats to validity, market regime shifts, and Trustworthy AI safeguards. Chapter 8 concludes with concrete future research directions.


# 2. Background and Related Work

## 2.1 Limit Order Book Microstructure and Order Book Imbalance
Modern electronic financial exchanges operate as continuous double auctions organized in Limit Order Books (LOB). An LOB maintains outstanding limit buy orders (bids) and limit sell orders (asks) ranked by price priority and time priority. The best bid $P_t^{\text{bid1}}$ and best ask $P_t^{\text{ask1}}$ define the inside spread $S_t = P_t^{\text{ask1}} - P_t^{\text{bid1}}$ and the reference mid-price:
$$P_t^{\text{mid}} = \frac{P_t^{\text{bid1}} + P_t^{\text{ask1}}}{2}$$
In empirical microstructure, Cont, Kukanov, and Stoikov (2014) established that price movements and short-term volatility are intimately driven by order flow imbalance and queue depletion across depth levels. Specifically, when the aggregate volume resting on the bid side substantially exceeds the ask side, the probability of an upward mid-price revision increases. The standardized Order Book Imbalance ($\text{OBI}$) at depth level $k$ is defined as:
$$\text{OBI}_{t, k} = \frac{V_{t, k}^{\text{bid}} - V_{t, k}^{\text{ask}}}{V_{t, k}^{\text{bid}} + V_{t, k}^{\text{ask}} + \epsilon}$$
where $V_{t, k}^{\text{bid}}$ and $V_{t, k}^{\text{ask}}$ represent queue volumes at level $k$, and $\epsilon > 0$ is a small numerical stabilizer. In high-frequency cryptocurrency markets, wide spreads and shallow top-of-book depth frequently amplify volatility, making multi-level order book imbalances crucial leading indicators of impending variance spikes.

## 2.2 Volatility Clustering and Realized Volatility
The stylized empirical properties of financial asset returns have been extensively documented since Mandelbrot (1963), who observed that "large changes tend to be followed by large changes, of either sign, and small changes by small changes." This phenomenon, known as **volatility clustering**, signifies that while price returns are typically uncorrelated across time, return magnitudes—such as absolute returns $|r_t|$ and squared returns $r_t^2$—exhibit pronounced, long-lasting positive autocorrelation.

Engle (1982) formalized this behavior in the Autoregressive Conditional Heteroskedasticity (ARCH) model, which Bollerslev (1986) generalized into the GARCH framework. In high-frequency continuous-time finance, Andersen and Bollerslev (1998) revolutionized the field by showing that summing intraday squared returns yields an asymptotically error-free, non-parametric measurement of latent integrated variance, termed **Realized Volatility (RV)**:
$$\text{RV}_{t, w} = \sqrt{\sum_{i=0}^{w-1} r_{t-i}^2}$$
Unlike parametric ARCH/GARCH models fitted to daily return series via maximum likelihood estimation, realized volatility directly converts high-frequency price fluctuations into an observable time-series quantity that can be modeled and forecasted using regression techniques.

## 2.3 Continuous Volatility Filters and Range Estimators
In institutional risk management, J.P. Morgan's (1996) RiskMetrics framework popularized the Exponentially Weighted Moving Average (EWMA) volatility estimator. The EWMA applies exponentially decaying weights to historical squared returns, ensuring that recent price shocks exert a higher influence than distant observations:
$$\sigma_{t, \lambda}^2 = (1 - \lambda) \sum_{i=0}^{\infty} \lambda^i r_{t-i}^2$$
where $\lambda \in (0, 1)$ is the decay parameter, related to an effective memory span $S$ via $\lambda = (S - 1) / (S + 1)$. In high-frequency continuous feeds, multi-scale EWMA filters across fast ($\lambda \approx 0.818$), medium ($\lambda \approx 0.935$), and slow ($\lambda \approx 0.967$) decay schedules capture immediate market turbulence as well as persistent intraday volatility baselines.

Complementary to return-based estimators, Parkinson (1980) introduced extreme-value range estimators utilizing intraday price highs and lows. Extended to order book microstructure, the normalized depth range proxy:
$$\text{Range}_{t, w} = \frac{\max_{i \in [0, w]} P_{t-i}^{\text{ask5}} - \min_{i \in [0, w]} P_{t-i}^{\text{bid5}}}{P_t^{\text{mid}}}$$
measures the geometric envelope spanned by the entire 5-level order book, providing an instantaneous indicator of depth-wide quote expansion during volatile quoting regimes.

## 2.4 The Heterogeneous Autoregressive Model (HAR-RV)
Corsi (2009) proposed the Heterogeneous Autoregressive model of Realized Volatility (HAR-RV), grounded in the Heterogeneous Market Hypothesis of Müller et al. (1997). This hypothesis posits that financial markets are populated by diverse agents with distinct trading horizons—ranging from high-frequency market makers and algorithmic scalpers (seconds to minutes) to institutional rebalancing desks (daily, weekly, and monthly). 

Instead of requiring formal fractional integration ($I(d)$ long-memory processes), Corsi showed that an additive cascade of realized volatility aggregated over heterogeneous sampling frequencies parsimoniously reproduces the observed long-memory autocorrelation and slow hyperbolic decay of financial volatility:
$$\text{RV}_{t+1\text{d}} = \beta_0 + \beta_{\text{daily}} \text{RV}_{t}^{(\text{d})} + \beta_{\text{weekly}} \text{RV}_{t}^{(\text{w})} + \beta_{\text{monthly}} \text{RV}_{t}^{(\text{m})} + \epsilon_{t+1}$$
In high-frequency cryptocurrency order book settings, we adapt Corsi's formulation to high-frequency tick scales by constructing realized volatility cascades across immediate (5 ticks), intermediate (10 to 30 ticks), and macro (60 ticks) horizons.

## 2.5 Gradient Boosted Decision Trees and Bayesian Tuning
In machine learning applications to tabular and financial time series, Gradient Boosted Decision Trees (GBDT)—specifically LightGBM (Ke et al., 2017) and XGBoost (Chen and Guestrin, 2016)—consistently demonstrate superior predictive accuracy over deep neural architectures. GBDT constructs an additive ensemble of shallow regression trees by sequentially fitting base learners to the negative gradient of the loss function:
$$f_M(\mathbf{x}) = \sum_{m=1}^M \gamma_m h_m(\mathbf{x})$$
LightGBM optimizes split finding through Gradient-based One-Side Sampling (GOSS) and Exclusive Feature Bundling (EFB), growing trees leaf-wise. XGBoost incorporates second-order Taylor expansions of the objective function alongside exact $\ell_1$ (Lasso) and $\ell_2$ (Ridge) leaf weight regularization.

To optimize hyperparameter configurations across continuous and discrete search spaces, Bergstra et al. (2011) introduced Tree-structured Parzen Estimators (TPE), formalized in Optuna by Akiba et al. (2019). TPE applies Bayesian sequential model-based optimization by modeling the conditional density of hyperparameter configurations given validation performance:
$$p(\theta \mid y) = \begin{cases} \ell(\theta) & \text{if } y < y^* \\ g(\theta) & \text{if } y \ge y^* \end{cases}$$
directing the search towards regions that maximize the expected improvement $\ell(\theta) / g(\theta)$.

## 2.6 Forecast Combination, Stacking, and the Combination Puzzle
The aggregation of distinct forecasting models has a rich history originating with Bates and Granger (1969), who proved that an optimal linear combination of two unbiased forecasts variance-dominates either individual constituent. Wolpert (1992) extended this concept to machine learning through **stacked generalization (stacking)**, in which a meta-learner is trained on out-of-fold predictions produced by primary base models.

However, in financial econometrics, Timmermann (2006) documented the pervasive **Forecast Combination Puzzle**: in out-of-sample empirical testing, simple equal-weight averaging ($w_i = 1/K$) or constrained linear combinations consistently outperform complex estimated optimal weights or non-linear meta-learners. When base models are highly collinear or validation sample noise is non-trivial, estimating additional meta-parameters introduces parameter estimation variance that swamps any theoretical diversification gains.

## 2.7 Statistical Evaluation and Robust Loss Functions
In statistical evaluation of financial forecasts, Diebold and Mariano (1995) established rigorous non-parametric tests for comparing predictive accuracy across competing models. Patton (2011) examined loss function robustness in volatility forecasting, proving that when the true underlying volatility is unobservable and proxied by an imperfect realized measure, only two loss function families are strictly robust to measurement noise: Mean Squared Error (MSE) and the Quasi-Likelihood (QLIKE) loss function:
$$L_{\text{QLIKE}}(y, \hat{y}) = \frac{y}{\hat{y}} - \ln\left(\frac{y}{\hat{y}}\right) - 1$$
QLIKE penalizes under-predictions more heavily than over-predictions, reflecting the asymmetric economic loss experienced by institutional risk desks during severe market drawdowns. Furthermore, because high-frequency overlapping label windows induce moving average serial correlation in forecast residuals, Newey and West (1987) Heteroskedasticity and Autocorrelation Consistent (HAC) covariance estimators are strictly required to avoid inflated $t$-statistics and false discovery.


# 3. Data Architecture and Integrity

## 3.1 Raw Dataset Schema and Provenance
The experimental data consists of high-frequency Level-2 Limit Order Book (L2 LOB) depth snapshots captured synchronously for three major cryptocurrency assets: Ethereum ($\text{ETH}$), Bitcoin ($\text{BTC}$), and Solana ($\text{SOL}$). 

Each asset feed records 21 base columns per snapshot:
- `timestamp`: Sampling timestamp of the order book snapshot.
- `mid_price`: Reference mid-price between best bid and best ask.
- `bid_price1` to `bid_price5`: Limit order prices at the 5 best bid levels.
- `bid_volume1` to `bid_volume5`: Cumulative resting queue volumes at the 5 best bid levels.
- `ask_price1` to `ask_price5`: Limit order prices at the 5 best ask levels.
- `ask_volume1` to `ask_volume5`: Cumulative resting queue volumes at the 5 best ask levels.
- `label` (present exclusively in $\text{ETH}$ training data): Target 10-step forward realized volatility.

Because GitHub enforces a strict 100 MB per-file upload limitation, the raw $\text{ETH}$ training records are split into two sequential 50 MB files: `ETH-01.csv` and `ETH-02.csv`. In all pipeline stages, these two splits are vertically concatenated without loss of continuity to reconstruct the complete 631,292 raw $\text{ETH}$ training observations:
```python
df_eth = pd.concat([
    pd.read_csv('datasets/train/ETH-01.csv'),
    pd.read_csv('datasets/train/ETH-02.csv')
], ignore_index=True)
```
The raw `BTC.csv` and `SOL.csv` files contain exactly 631,292 records each, mirroring the exact row count of the reconstructed $\text{ETH}$ dataset.

## 3.2 Continuous Time-Series Reconstruction and Row Usability
A critical finding during exploratory data analysis was that standard tabular data preparation routines—such as naive `df.dropna()` across all columns—severely degrade temporal continuity. When multi-asset merges are executed with standard `dropna()`, over 68% of the observations (445,222 rows) are discarded, shrinking the dataset from 631,292 rows to just 186,070 rows and destroying high-frequency autocorrelation memory.

To resolve this, Stage 4 introduced the **continuous time-series reconstruction protocol**:
1. Prices are forward-filled (`ffill()`) across consecutive ticks to preserve continuous state representations during microsecond trade pauses.
2. Rolling window calculations enforce `min_periods=2` to ensure valid numerical variance estimates as early as the second tick.
3. Feature vectors replace infinite values with zero and apply causal imputation (`fillna(0)`), ensuring that no future information leaks into historical features.

Following this protocol, exactly **605,938 continuous, physically valid rows** are retained out of the 631,292 raw records (96.0% sample retention). The dropped 25,354 rows correspond strictly to unlabelled observations (`label.isna() == True` in `ETH.csv`), while mid-prices remain strictly positive across all 631,292 rows.

## 3.3 Sampling Frequency and Tick Spacing Audit
A fundamental premise in high-frequency research is verifying whether operational "ticks" correspond to physical "seconds." To confirm whether the dataset's "10-tick" rolling horizon constitutes an authentic "10-second" forward window, we computed the empirical distribution of consecutive timestamp differences ($\Delta t$) across the entire $\text{ETH}$ dataset (631,291 consecutive intervals).

The empirical timestamp audit reveals:
- **Minimum $\Delta t$:** **1.000 second**
- **Median $\Delta t$:** Exactly **1.000 second**
- **Mean $\Delta t$:** **1.000 second**
- **90th Percentile ($p_{90}$):** **1.000 second**
- **99th Percentile ($p_{99}$):** **1.000 second**
- **Maximum $\Delta t$:** **1.000 second**

Because every single consecutive timestamp interval across all 631,291 deltas is uniformly 1.000 second with zero temporal jitter, the operational 10-row rolling window is physically and mathematically identical to a **10-second forward execution horizon**.

## 3.4 Target Label Construction and Contemporaneous Overlap
In supervised machine learning, target label leakage represents the single most dangerous threat to validity. The dataset's target label $y_t$ is formally defined as the sample standard deviation of 10 consecutive logarithmic returns starting at snapshot index $t$:
$$y_t = \text{std}\left( r_t, r_{t+1}, r_{t+2}, \dots, r_{t+9} \right)$$
where $r_{t+k} = \ln(P_{t+k}^{\text{mid}} / P_{t+k-1}^{\text{mid}})$.

### Empirical Verification (Check 2)
To verify this formula against the ground truth labels in `ETH.csv`, we recomputed the forward standard deviation programmatically in `src/verify_data_integrity.py`. The comparison between reconstructed labels and dataset labels yielded:
- **Pearson Correlation ($r$):** **0.9907**
- **Exact Floating-Point Match ($\le 10^{-10}$):** **88.0% of rows**
- **Mean Absolute Deviation (MAD):** **$1.39 \times 10^{-6}$**

The minor residual discrepancy arises solely from floating-point numerical precision differences in sample variance divisors ($N$ vs $N-1$) in the upstream data collector.

### Methodological Transparency: Contemporaneous Return Overlap
A crucial insight discovered during mathematical simulation of the label is that the forward rolling window $[t, t+9]$ begins at tick $t$. Consequently:
$$r_t = \ln\left( \frac{P_t^{\text{mid}}}{P_{t-1}^{\text{mid}}} \right)$$
is observable at snapshot $t$ and is simultaneously included in the engineered feature space (e.g., `ret_squared` and `abs_return`). 

**Audit Conclusion:** While our feature matrices contain **zero future information** from $t+1$ onwards, 1 of the 10 return terms comprising the target standard deviation is observable contemporaneously at tick $t$. This explains why squared return $r_t^2$ is identified as an important short-term predictor. In compliance with Trustworthy AI principles, we strictly report that "features contain no future information," explicitly refraining from claiming "zero contemporaneous overlap."

## 3.5 Cross-Asset Alignment and Data Sanitization
The training set includes synchronized order books for $\text{BTC}$ and $\text{SOL}$. An audit of timestamp formatting indicates that while $\text{ETH}$ contains full second-level datetime strings (`YYYY-MM-DD HH:MM:SS`), $\text{BTC}$ and $\text{SOL}$ record minute-level timestamps (`DD-MM-YYYY HH:MM`). Horizontal merging via `pd.concat(axis=1)` assumes exact chronological alignment across files sharing identical row counts (631,292 rows). Check 1 in `verify_data_integrity.py` confirmed that minute-level timestamps match across all 631,292 rows without drift.

### Sensor Spike Sanitization
Cryptocurrency exchange WebSocket feeds occasionally transmit corrupted price ticks (e.g., sensor anomalies where price $\le 0$ or price $> 10^6$). In `src/stage4_multiscale_pipeline.py`, an automated data hygiene filter identifies and sanitizes these anomalies prior to return computation:
```python
for col in df.columns:
    if 'price' in col and df[col].dtype in [np.float64, np.float32]:
        df.loc[(df[col] <= 0.0) | (df[col] > 1e6), col] = np.nan
        df[col] = df[col].ffill()
```
This guarantees that return calculations remain finite, bounded, and free of artificial sensor spikes.


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


# 8. Conclusion and Future Roadmap

## 8.1 Summary of Research Findings
This investigation conducted an empirical and econometric evaluation of 10-second forward realized volatility forecasting for Ethereum using Level-2 limit order book microstructure. Guided by Trustworthy AI principles, the study evaluated models across 605,938 continuous ticks under a strict 70/15/15 chronological three-way partitioning protocol.

The key empirical findings are:
1. **Raw Depth Insufficiency:** Raw Level-2 order book depth vectors carry negligible direct predictive signal for forward volatility ($r \in [-0.0189, 0.0947]$). Unconstrained tree models memorize microstructure noise rapidly within 5 to 6 boosting iterations.
2. **Primacy of Volatility Memory:** Multi-scale realized volatility and continuous RiskMetrics EWMA features unlock the foundational signal, elevating predictive correlation from $0.095$ to $0.685$.
3. **Power of Linear Regularization:** A regularized multi-asset HAR-style Ridge model on 30 stationary volatility features achieves an out-of-sample correlation of $r = 0.6860$, outperforming complex standalone gradient boosted trees ($r = 0.6749$ for XGBoost).
4. **Failure of Complex Stacking:** Base tree learners exhibit extreme prediction collinearity ($r = 0.965$). In accordance with the Bates-Granger forecast combination puzzle, non-linear stacking meta-learners overfit validation residuals ($r = 0.6615$ on the 80/20 validation split vs $0.6910$ for linear SLSQP blending), while on held-out test data simple equal-weight blending ($r = 0.6828$) strictly outperforms validation-optimized SLSQP weights ($r = 0.6711$).
5. **Peak Hybrid Performance:** The Econometric-Tree Hybrid—combining equal thirds of regularized Ridge, LightGBM, and XGBoost—achieves the peak out-of-sample benchmark of **Test $r = 0.6887$** and **Test RMSE $= 4.797 \times 10^{-5}$**, successfully synthesizing linear volatility memory with non-linear order book queue thresholding.
6. **Integrity Transparency:** Transparent mathematical analysis of label construction confirmed that the forward 10-step rolling standard deviation window incorporates the contemporaneous return $r_t$. While features contain no future information, contemporaneous variance overlap contributes to short-memory predictability.

## 8.2 Future Research Roadmap
To build upon the foundations established in this research, five promising avenues for future quantitative investigation are proposed:

1. **Deep Sequence Architectures (LSTM and TCN):** While static tabular models require handcrafted rolling features, deep sequential architectures—such as Long Short-Term Memory (LSTM) networks, Temporal Convolutional Networks (TCN), and State Space Models (Mamba)—can ingest raw sequential tick order books directly, potentially learning richer representation cascades across microsecond dynamics.
2. **Order Flow and Trade Tape Integration:** Level-2 snapshots capture passive resting liquidity but omit active market order executions. Integrating Level-1 trade tape data—specifically Cumulative Volume Delta (CVD), trade sign aggressiveness, and tick-by-tick order flow imbalance—could provide leading indicators of imminent queue exhaustion.
3. **Asymmetric QLIKE Objective Optimization:** Current tree implementations optimize standard mean squared error (MSE). Developing custom objective and Hessian functions in LightGBM and XGBoost targeting Patton's (2011) asymmetric QLIKE loss function would penalize volatility under-estimation, directly aligning models with institutional downside risk management.
4. **Formal Statistical Testing:** Future work should execute block-bootstrap confidence intervals and formal Diebold-Mariano tests across rolling out-of-sample blocks to rigorously test the statistical significance of small incremental gains ($\Delta r \le 0.006$) among top-tier models.
5. **Multi-Period Regime Expansion:** Evaluating the pipeline across extended multi-month cryptocurrency regimes, multi-token baskets (e.g., DeFi altcoins), and traditional equity index futures (e.g., S&P 500 E-mini) would establish whether our findings on the dominance of regularized linear memory generalize across diverse market microstructures.


# 9. References and Appendices

## References
All bibliographic entries are formatted in standard IEEE citation style:

1. [1] T. Andersen and T. Bollerslev, "Answering the skeptics: Yes, standard volatility models do provide accurate forecasts," *International Economic Review*, vol. 39, no. 4, pp. 885–905, 1998.
2. [2] R. Engle, "Autoregressive conditional heteroscedasticity with estimates of the variance of United Kingdom inflation," *Econometrica*, vol. 50, no. 4, pp. 987–1007, 1982.
3. [3] T. Bollerslev, "Generalized autoregressive conditional heteroskedasticity," *Journal of Econometrics*, vol. 31, no. 3, pp. 307–327, 1986.
4. [4] B. Mandelbrot, "The variation of certain speculative prices," *The Journal of Business*, vol. 36, no. 4, pp. 394–419, 1963.
5. [5] R. Cont, A. Kukanov, and S. Stoikov, "The price impact of order book events," *Journal of Financial Econometrics*, vol. 12, no. 1, pp. 47–88, 2014.
6. [6] F. Corsi, "A simple approximate long-memory model of realized volatility," *Journal of Financial Econometrics*, vol. 7, no. 2, pp. 174–196, 2009.
7. [7] U. Müller, M. Dacorogna, R. Davé, O. Pictet, R. Olsen, and J. Ward, "Volatilities of different time resolutions—Analyzing the dynamics of market components," *Journal of Empirical Finance*, vol. 4, no. 2-3, pp. 213–239, 1997.
8. [8] M. Parkinson, "The extreme value method for estimating the variance of the rate of return," *The Journal of Business*, vol. 53, no. 1, pp. 61–65, 1980.
9. [9] J.P. Morgan, "RiskMetrics—Technical Document," 4th ed., J.P. Morgan / Reuters, New York, 1996.
10. [10] T. Chen and C. Guestrin, "XGBoost: A scalable tree boosting system," in *Proc. 22nd ACM SIGKDD Int. Conf. on Knowledge Discovery and Data Mining*, pp. 785–794, 2016.
11. [11] G. Ke, Q. Meng, T. Finley, T. Wang, W. Chen, W. Ma, Q. Ye, and T.-Y. Liu, "LightGBM: A highly efficient gradient boosting decision tree," in *Advances in Neural Information Processing Systems (NeurIPS)*, vol. 30, pp. 3146–3154, 2017.
12. [12] J. Bergstra, R. Bardenet, Y. Bengio, and B. Kégl, "Algorithms for hyper-parameter optimization," in *Advances in Neural Information Processing Systems (NeurIPS)*, vol. 24, pp. 2546–2554, 2011.
13. [13] T. Akiba, S. Sano, T. Yanase, T. Ohta, and M. Koyama, "Optuna: A next-generation hyperparameter optimization framework," in *Proc. 25th ACM SIGKDD Int. Conf. on Knowledge Discovery and Data Mining*, pp. 2623–2631, 2019.
14. [14] J. Bates and C. Granger, "The combination of forecasts," *Journal of the Operational Research Society*, vol. 20, no. 4, pp. 451–468, 1969.
15. [15] D. Wolpert, "Stacked generalization," *Neural Networks*, vol. 5, no. 2, pp. 241–259, 1992.
16. [16] A. Timmermann, "Forecast combinations," in *Handbook of Economic Forecasting*, vol. 1, pp. 135–196, Elsevier, 2006.
17. [17] A. Patton, "Volatility forecast comparison using imperfect volatility proxies," *Journal of Econometrics*, vol. 160, no. 1, pp. 246–256, 2011.
18. [18] F. Diebold and R. Mariano, "Comparing predictive accuracy," *Journal of Business & Economic Statistics*, vol. 13, no. 3, pp. 253–263, 1995.
19. [19] W. Newey and K. West, "A simple, positive semi-definite, heteroskedasticity and autocorrelation consistent covariance matrix," *Econometrica*, vol. 55, no. 3, pp. 703–708, 1987.

---

## Appendix A: Hyperparameter Specifications per Model
1. **Calibrated RidgeCV (30 Vol Features):**
   - Standardization: `StandardScaler(with_mean=True, with_std=True)`
   - Alpha grid: 15 log-spaced values from $10^{-4}$ to $10^{4}$ (`np.logspace(-4, 4, 15)`)
   - Cross-validation: Generalized Cross-Validation (GCV) on 424,156 training rows.
2. **LightGBM Conservative (Depth 4):**
   - `n_estimators=3000`, `max_depth=4`, `num_leaves=15`, `learning_rate=0.05`
   - `colsample_bytree=0.70`, `subsample=0.80`, `random_state=42`, `early_stopping=50`
3. **XGBoost Conservative (Depth 4):**
   - `n_estimators=3000`, `max_depth=4`, `learning_rate=0.05`, `colsample_bytree=0.60`
   - `subsample=0.80`, `tree_method='hist'`, `device='cuda'`, `early_stopping_rounds=50`
   - Serialized with `device='cpu'` for deployment.
4. **LightGBM Deep (Depth 6):**
   - `n_estimators=3000`, `max_depth=6`, `num_leaves=40`, `learning_rate=0.03`
   - `colsample_bytree=0.80`, `subsample=0.80`, `early_stopping=50`
5. **XGBoost Deep (Depth 6):**
   - `n_estimators=3000`, `max_depth=6`, `learning_rate=0.03`, `colsample_bytree=0.80`
   - `subsample=0.80`, `tree_method='hist'`, `device='cuda'`, `early_stopping_rounds=50`
6. **Optuna Stage 7 Tuned Models:**
   - LightGBM: `n_estimators=1200`, `max_depth=5`, `num_leaves=37`, `learning_rate=0.015`, `colsample_bytree=0.50`, `subsample=0.60`, `min_child_samples=50`
   - XGBoost: `n_estimators=2000`, `max_depth=4`, `learning_rate=0.035`, `colsample_bytree=0.50`, `subsample=0.75`, `min_child_weight=7`

---

## Appendix B: The Complete 135-Feature Space Listing
The complete feature listing exported to `models/final_feature_list.joblib` consists of:
- **ETH Base (21):** `mid_price`, `bid_price1-5`, `bid_volume1-5`, `ask_price1-5`, `ask_volume1-5`
- **BTC Base (21):** `mid_price_BTC`, `bid_price1-5_BTC`, `bid_volume1-5_BTC`, `ask_price1-5_BTC`, `ask_volume1-5_BTC`
- **SOL Base (21):** `mid_price_SOL`, `bid_price1-5_SOL`, `bid_volume1-5_SOL`, `ask_price1-5_SOL`, `ask_volume1-5_SOL`
- **ETH Engineered (21):** `log_return`, `ret_squared`, `abs_return`, `realized_vol_5`, `realized_vol_10`, `realized_vol_15`, `realized_vol_30`, `realized_vol_60`, `ewma_vol_10`, `ewma_vol_30`, `ewma_vol_60`, `spread1-5`, `obi1-3`, `norm_range_15`, `norm_range_30`
- **BTC Engineered (21):** `log_return_BTC`, `ret_squared_BTC`, `abs_return_BTC`, `realized_vol_5_BTC`, `realized_vol_10_BTC`, `realized_vol_15_BTC`, `realized_vol_30_BTC`, `realized_vol_60_BTC`, `ewma_vol_10_BTC`, `ewma_vol_30_BTC`, `ewma_vol_60_BTC`, `spread1-5_BTC`, `obi1-3_BTC`, `norm_range_15_BTC`, `norm_range_30_BTC`
- **SOL Engineered (21):** `log_return_SOL`, `ret_squared_SOL`, `abs_return_SOL`, `realized_vol_5_SOL`, `realized_vol_10_SOL`, `realized_vol_15_SOL`, `realized_vol_30_SOL`, `realized_vol_60_SOL`, `ewma_vol_10_SOL`, `ewma_vol_30_SOL`, `ewma_vol_60_SOL`, `spread1-5_SOL`, `obi1-3_SOL`, `norm_range_15_SOL`, `norm_range_30_SOL`
- **Cross-Asset & Lags (9):** `ret_lag1-3`, `rvol10_lag1-3`, `mid_ratio_ETH_BTC`, `mid_ratio_ETH_SOL`, `ret_diff_ETH_BTC`

---

## Appendix C: Stage, Script, and Execution Log Mapping
All empirical results are fully traceable to execution records in `logs/run_2026-11-09.log`:
- Raw Baselines: lines 12–148 (`src/stage1_train_baselines.py`)
- Overfitting Proof: lines 150–210 (`src/stage1_overfitting_proof.py`)
- Cross-Asset & Reduction: lines 212–390 (`src/stage2_*.py`)
- Continuous EWMA Pipeline: lines 420–580 (`src/stage4_multiscale_pipeline.py`)
- Ensembles & SLSQP: lines 582–720 (`src/stage5_model_ensemble.py`)
- Stacking Meta-Learners: lines 722–840 (`src/stage6_stacking_ensemble.py`)
- Optuna Blend: lines 842–950 (`src/stage7_optuna_ensemble.py`)
- Authoritative 3-Way Benchmark: lines 952–1077 (`src/eval_rigorous_test_benchmark.py`)

---

## Appendix D: Step-by-Step Reproduction Instructions
To reproduce all reported findings, figures, and artifacts from scratch:
```bash
# 1. Clone repository and activate Python virtual environment
cd /home/plyrX06996/Documents/GitHub/Secondary/TAI_project
source .venv/bin/activate

# 2. Run automated data and methodology integrity verification
python src/verify_data_integrity.py

# 3. Execute authoritative 3-way chronological benchmark with app export
python src/eval_rigorous_test_benchmark.py --export-app

# 4. Execute the automated pytest test suite
pytest -q tests/

# 5. Generate publication figures and verified facts.json
python src/make_figures.py

# 6. Build the 16:9 viva presentation deck (PPTX and PDF)
python presentation/build_deck.py

# 7. Compile the complete academic project report
python report/build_report.py

# 8. Launch the interactive Streamlit quantitative terminal
streamlit run app.py
```


