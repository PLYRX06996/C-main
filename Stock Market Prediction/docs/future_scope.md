# Future Scope & Algorithmic Roadmap

**From Tabular Econometrics to Deep Sequential Volatility Modeling**

---

## 1. Methodological Retrospective: Why Classical ARIMA Was Excluded

During the research design phase, time-series benchmarks were evaluated against project constraints (3-day delivery timeline, 605,938 continuous tick records, NVIDIA RTX 2050 4GB GPU). The feasibility matrix below contrasts the evaluated options:

| Method / Architecture | Estimated Build Time | Execution Runtime | Academic & Econometric Impact | Decision / Status |
|---|---|---|---|---|
| **Dedicated HAR-RV / Extended AR Benchmark** | 30 minutes | ~5 seconds | **High:** Formalizes the Corsi (2009) econometric benchmark as a standalone script. | **Implemented & Verified** ($r = 0.6710$ canonical, $r = 0.6860$ Ridge-30vol) |
| **Lightweight PyTorch 1D-CNN or GRU** | 1.5 – 2 hours | ~15 minutes (RTX 2050) | **Very High:** Modern deep sequential comparison capturing non-linear temporal dynamics. | **Prioritized Future Scope (Phase 1)** |
| **Theoretical Analysis & Future Scope** | 15 minutes | 0 | **Sufficient:** Formulates why HAR-RV/trees suffice and specifies sequential roadmap. | **Formalized in this Document** |
| **Classical ARIMA (`statsmodels`)** | 1 hour | Crashes / Hours | **Negative / Inappropriate:** Structurally flawed for volatility; MLE chokes on 630K rows. | **Strictly Avoided** |

### Statistical and Computational Reasons Classical ARIMA Fails
Classical Box-Jenkins Autoregressive Integrated Moving Average (ARIMA) models were deliberately excluded based on two fatal theoretical and computational barriers:
1. **Mean vs. Variance Specification:** ARIMA is mathematically formulated to model the conditional *mean* of a time series:
   $$\Phi(B)(1 - B)^d X_t = \Theta(B) \epsilon_t$$
   In high-frequency cryptocurrency markets, returns $r_t$ have an unconditional mean indistinguishable from zero ($\mathbb{E}[r_t] \approx 0$). Volatility, by contrast, is a strictly positive, non-linear second-moment process ($\mathbb{E}[r_t^2 \mid \mathcal{F}_{t-1}]$) characterized by conditional heteroskedasticity, volatility clustering (Mandelbrot, 1963; Engle, 1982), and asymmetric leverage. Applying ARIMA directly to volatility or squared returns assumes additive Gaussian noise, ignoring the log-normal or skewed distribution of realized variance.
2. **Computational Intractability on High-Frequency Data:** Standard implementations (such as `statsmodels.tsa.arima.model.ARIMA`) optimize parameters via numerical Maximum Likelihood Estimation (MLE) using state-space Kalman filters. On a dataset of **605,938 continuous rows**, inverting dense covariance matrices at every numerical optimization step causes out-of-memory errors, non-invertible Hessian warnings, or execution timeouts exceeding several hours.
3. **Hyperbolic Long-Memory Decay:** Autoregressive Moving Average processes exhibit exponential autocorrelation decay ($\sim e^{-ck}$). Financial realized volatility exhibits slow, hyperbolic power-law decay ($\sim k^{-\alpha}$). Capturing this decay in the Box-Jenkins framework requires fractional integration (ARFIMA), which is even more computationally prohibitive on 600K rows.

### Why Corsi's (2009) HAR-RV Was the Superior Solution
Corsi's Heterogeneous Autoregressive model circumvents all ARIMA limitations:
- It models realized volatility directly rather than return means.
- It reproduces hyperbolic long-memory persistence through an additive cascade of heterogeneous time horizons (fast, medium, slow).
- It is estimated via closed-form Ordinary Least Squares (OLS) in **under 5 seconds** on 424,156 training rows without numerical optimization divergence.

---

## 2. Phase 1 Roadmap: Lightweight PyTorch Sequence Models

While tabular gradient boosted trees and regularized linear regressions achieve strong out-of-sample correlation ($r = 0.6887$), they treat each tick snapshot as a static row, requiring handcrafted rolling windows to simulate memory. The primary next milestone is implementing lightweight deep learning sequence models in PyTorch that process raw temporal sequences directly.

### 2.1 Dilated Temporal Convolutional Networks (1D-CNN)
Temporal Convolutional Networks (TCN) with causal dilated 1D convolutions provide an optimal architecture for high-frequency tick feeds:
- **Causal Convolutions:** Enforce zero lookahead leakage ($y_t$ depends only on inputs $x_{\le t}$).
- **Dilated Receptive Field:** Dilation factors $d \in \{1, 2, 4, 8, 16, 32\}$ expand the temporal receptive field exponentially without increasing parameter counts or computing recurrent state steps.
- **Hardware Efficiency:** Fully parallelizable on consumer GPUs (NVIDIA GeForce RTX 2050 4GB VRAM), requiring ~15 minutes training time.

$$\hat{y}_t = \text{Conv1D}\left( \mathbf{X}_{t-K:t}, \mathbf{W}, d \right)$$

### 2.2 Gated Recurrent Units (GRU)
For recurrent sequential modeling, Gated Recurrent Units (GRU) offer a streamlined alternative to 3-gate LSTMs:
- **Reset and Update Gates:** Efficiently manage long-term volatility baselines alongside sudden microsecond order book replenishment.
- **Memory Footprint:** 25% fewer parameters than LSTM, avoiding CUDA out-of-memory errors on 4GB VRAM.

$$\mathbf{h}_t = (1 - \mathbf{z}_t) \odot \mathbf{h}_{t-1} + \mathbf{z}_t \odot \tilde{\mathbf{h}}_t$$

---

## 3. Phase 2 Roadmap: Attention Mechanisms & State-Space Models

For larger compute clusters or multi-asset extensions, advanced transformer and state-space architectures represent the frontier:

1. **Temporal Fusion Transformer (TFT):**
   - Applies multi-head self-attention across time steps while utilizing specialized gating networks (Gated Residual Networks) to select the most relevant Level-2 depth features dynamically.
   - Provides interpretable attention weights showing which historical ticks drove a volatility forecast.
2. **Mamba / Structured State-Space Models (SSM):**
   - Linear-time complexity $\mathcal{O}(L)$ in sequence length $L$, allowing training on continuous un-downsampled tick sequences of 10,000+ steps without the quadratic memory bottleneck of standard Transformers ($\mathcal{O}(L^2)$).

---

## 4. Loss Function and Statistical Testing Innovations

1. **Custom Asymmetric QLIKE Loss Training:**
   - Current tree models minimize MSE during leaf splitting. Implementing custom gradient and Hessian functions for Patton's (2011) QLIKE loss will penalize volatility under-predictions, directly optimizing models for downside institutional risk.
   - First and second analytical derivatives for custom GBDT objectives:
     $$g_i = \frac{\partial L}{\partial \hat{y}_i} = \frac{1}{\hat{y}_i} - \frac{y_i}{\hat{y}_i^2}, \quad h_i = \frac{\partial^2 L}{\partial \hat{y}_i^2} = \frac{2 y_i}{\hat{y}_i^3} - \frac{1}{\hat{y}_i^2}$$
2. **Formal Block-Bootstrap & Diebold-Mariano Tests:**
   - Implement stationary block bootstrapping (Politis & Romano, 1994) to construct empirical 95% confidence intervals for out-of-sample correlation differences among top models (Hybrid $0.6887$ vs Ridge $0.6860$ vs Tree Blend $0.6828$), formally verifying statistical significance.
3. **High-Frequency Order Flow Tape Integration:**
   - Incorporate tick-level market order executions (Level-1 trade tape), Cumulative Volume Delta (CVD), and microsecond trade aggressiveness to capture active order exhaustion before passive quote revisions.
