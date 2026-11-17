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
