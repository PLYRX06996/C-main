# Data Dictionary & Schema Specification

The dataset contains high-frequency Level-2 Limit Order Book (L2 LOB) snapshots for three major cryptocurrency pairs (ETH, BTC, SOL) captured concurrently.

### Dataset Overview & Sample Sizes
- **Raw Observations:** 631,292 rows per asset.
- **Continuous Usable Matrix:** 605,938 rows (after price sanitization, forward filling, and dropping target NaN rows).
- **Sampling Frequency:** Verified exact median and mean tick delta of $\Delta t = 1.0$ second.
- **Unlabelled Test Partition:** `data/raw/test/` contains ~270,000 observations without labels, reserved for blind out-of-sample evaluation.

### Data Provisioning & Splitting
Due to GitHub's 100MB per-file upload limit, the raw training Ethereum dataset is split into two halves:
- `ETH-01.csv` (~52 MB)
- `ETH-02.csv` (~56 MB)

The pipeline automatically concatenates them in memory (`pd.concat([df1, df2], ignore_index=True)`), or they can be reconstructed via `reconstruct_eth.sh`.

### Order Book Columns (Per Asset: ETH, BTC, SOL)
- `timestamp`: Snapshot timestamp. ETH includes seconds/subseconds; BTC and SOL are formatted as `DD-MM-YYYY HH:MM`.
- `mid_price`: Midpoint between top bid and top ask:
  $$\text{mid\_price} = \frac{\text{bid\_price1} + \text{ask\_price1}}{2}$$
- `bid_price1` to `bid_price5`: Top 5 queued bid price levels (descending priority).
- `bid_volume1` to `bid_volume5`: Queued limit liquidity volume at each bid level.
- `ask_price1` to `ask_price5`: Top 5 queued ask price levels (ascending priority).
- `ask_volume1` to `ask_volume5`: Queued limit liquidity volume at each ask level.

### Target Variable (`label`)
- Present in training Ethereum dataset (`ETH-01.csv` / `ETH-02.csv`).
- **Mathematical Definition:** Forward 10-second rolling sample standard deviation of logarithmic mid-price returns:
  $$\sigma_{10s} = \sqrt{\frac{1}{9} \sum_{i=0}^9 (r_{t+i} - \bar{r})^2}, \quad r_{t+i} = \ln\left(\frac{P_{t+i}}{P_{t+i-1}}\right)$$
- **Integrity Note:** The 10-step window begins at contemporaneous return $r_t$. The feature matrix contains strictly past and contemporaneous information; no future information ($t+1 \dots t+9$) exists in features.

### Artifact Inventory & Legacy Tagging
- `models/stage4_multiscale_best.joblib`: Serialized Stage 4 continuous LightGBM model.
- `models/stage8_har_rv_best.joblib`: Serialized regularized multi-asset HAR-style Ridge model.
- `models/stage8_har_rv_results.csv`: Benchmark results across all HAR-RV configurations.
- `models/rigorous_benchmark_3way_results.csv`: Authoritative 70/15/15 3-way evaluation results.
- `models/*.pkl` *(Legacy)*: Early baseline serialized models retained for reproducibility.
- `models/v3_*` *(Legacy)*: Early Stage 3 Optuna exploratory artifacts.
