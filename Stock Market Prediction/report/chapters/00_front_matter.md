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
