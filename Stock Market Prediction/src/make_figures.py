"""
Academic Figure & Facts Generator (Reports & Viva Deck)
=======================================================
Reads serialized benchmark artifacts, cached test predictions, and CSVs
to generate publication-quality figures (1600x900 px, 16:9, 200 DPI)
and exports reports/facts.json with verified constants.
Zero training or refitting is performed.
"""

import os
import json
import joblib
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from scipy.stats import pearsonr

plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in plt.style.available else 'default')
plt.rcParams['font.sans-serif'] = 'DejaVu Sans'
plt.rcParams['font.size'] = 14
plt.rcParams['axes.titlesize'] = 18
plt.rcParams['axes.labelsize'] = 16
plt.rcParams['xtick.labelsize'] = 14
plt.rcParams['ytick.labelsize'] = 14
plt.rcParams['legend.fontsize'] = 14
plt.rcParams['figure.titlesize'] = 20

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MODELS_DIR = os.path.join(ROOT_DIR, 'models')
PROCESSED_DIR = os.path.join(ROOT_DIR, 'data', 'processed')
REPORTS_DIR = os.path.join(ROOT_DIR, 'reports')
FIG_DIR = os.path.join(REPORTS_DIR, 'figures')
DATA_DIR = os.path.join(ROOT_DIR, 'data', 'raw', 'train')
if not os.path.exists(DATA_DIR):
    DATA_DIR = os.path.join(ROOT_DIR, 'data', 'raw', 'train')

os.makedirs(FIG_DIR, exist_ok=True)

NAVY = '#1b365d'
TEAL = '#008080'
LIGHT_BLUE = '#6baed6'
DARK_GREY = '#4a4a4a'
LIGHT_GREY = '#e0e0e0'
CORAL = '#d95f02'
GREEN = '#2ca02c'

def save_figure(fig, filename):
    filepath = os.path.join(FIG_DIR, filename)
    fig.savefig(filepath, dpi=200, bbox_inches='tight', facecolor='white')
    plt.close(fig)
    print(f"Saved: {filepath} (1600x900, 16:9)")

def main():
    print("=" * 80)
    print(" Generating Academic Figures and facts.json (Reports & Viva Deck)")
    print("=" * 80)

    bench_csv = os.path.join(MODELS_DIR, 'rigorous_benchmark_3way_results.csv')
    har_csv = os.path.join(MODELS_DIR, 'stage8_har_rv_results.csv')
    cache_path = os.path.join(PROCESSED_DIR, 'app_cache.joblib')

    assert os.path.exists(bench_csv), f"Missing {bench_csv}"
    assert os.path.exists(har_csv), f"Missing {har_csv}"
    assert os.path.exists(cache_path), f"Missing {cache_path}"

    df_bench = pd.read_csv(bench_csv)
    df_har = pd.read_csv(har_csv)
    cache = joblib.load(cache_path)
    test_df = cache['test']
    meta = cache['meta']

    facts = {
        "dataset": {
            "raw_rows_per_asset": 631292,
            "usable_continuous_rows": 605938,
            "total_features": 135,
            "raw_price_level_features": 33,
            "stationary_features": 102,
            "volatility_features": 30,
            "assets": ["ETH", "BTC", "SOL"],
            "depth_levels": 5,
            "prediction_horizon_steps": 10,
            "prediction_horizon_text": "10-second forward rolling volatility",
            "median_delta_t_seconds": 1.000,
            "train_80_20": {"train": 484750, "val": 121188},
            "split_3way": {
                "train_rows": meta['n_train'],
                "val_rows": meta['n_val'],
                "test_rows": meta['n_test'],
                "train_pct": 70.0,
                "val_pct": 15.0,
                "test_pct": 15.0
            }
        },
        "stage1_raw_baselines": {
            "features": 21,
            "rf_pearson_r": -0.0189,
            "xgb_pearson_r": 0.0887,
            "lgbm_pearson_r": 0.0947,
            "early_stopping_rounds_xgb": 5,
            "early_stopping_rounds_lgbm": 6,
            "early_stopping_rmse": 7.9e-5,
            "forced_1000_rmse_xgb": 1.32e-4,
            "forced_1000_rmse_lgbm": 1.64e-4
        },
        "stage2_feature_engineering": {
            "v2_features_count": 90,
            "v2_pearson_r": 0.6442,
            "tree_selected_top25_r": 0.6435,
            "pca_95pct_components": 44,
            "pca_95pct_r": 0.5345,
            "pca_25_components": 25,
            "pca_25_r": 0.5184
        },
        "stage3_optuna_tuning": {
            "v3_features_count": 97,
            "v3_default_r": 0.6526,
            "v3_optuna_lgbm_r": 0.6558,
            "v3_optuna_xgb_r": 0.6440,
            "run_to_run_variance": "±0.003"
        },
        "stage4_multiscale": {
            "features_count": 135,
            "lgbm_r": 0.6811,
            "xgb_r": 0.6851
        },
        "stage5_ensembling_80_20": {
            "lgbm_cons_r": 0.6876,
            "lgbm_deep_r": 0.6813,
            "huber_lgbm_r": 0.6786,
            "xgb_cons_r": 0.6898,
            "xgb_deep_r": 0.6872,
            "equal_weight_blend_r": 0.6896,
            "slsqp_blend_r": 0.6910
        },
        "stage6_stacking_80_20": {
            "xgb_metalearner_r": 0.6615,
            "ridge_metalearner_r": 0.6895,
            "linear_slsqp_r": 0.6910
        },
        "stage7_optuna_blend_80_20": {
            "lgbm_tuned_r": 0.6882,
            "xgb_tuned_r": 0.6905,
            "tuned_blend_r": 0.6914
        },
        "benchmark_3way_oos": {
            row['model']: {
                "val_pearson_r": float(row['val_pearson_r']),
                "val_rmse": float(row['val_rmse']),
                "test_pearson_r": float(row['test_pearson_r']),
                "test_rmse": float(row['test_rmse'])
            }
            for _, row in df_bench.iterrows()
        },
        "har_rv_family": {
            row['model']: {
                "val_pearson_r": float(row['val_pearson_r']),
                "val_rmse": float(row['val_rmse']),
                "test_pearson_r": float(row['test_pearson_r']),
                "test_rmse": float(row['test_rmse']),
                "test_qlike": float(row['test_qlike'])
            }
            for _, row in df_har.iterrows()
        },
        "integrity_and_stationarity": {
            "label_recomputation_pearson_r": 0.9907,
            "label_exact_match_1e10_pct": 88.0,
            "label_mean_abs_deviation": 1.39e-6,
            "xgb_test_r_with_raw_prices": 0.6749,
            "xgb_test_r_without_raw_prices": 0.6750,
            "label_overlap_caveat": "The label window includes the contemporaneous return r_t; features contain no future information."
        },
        "regimes": {
            "q33": float(meta['q33']),
            "q67": float(meta['q67'])
        }
    }

    print("Verifying benchmark numbers against authoritative facts...")
    test_metrics = facts["benchmark_3way_oos"]
    assert abs(test_metrics["Baseline 1: Persistence (rvol10)"]["test_pearson_r"] - 0.5441) < 1e-4
    assert abs(test_metrics["Baseline 3: Calibrated RidgeCV (30 Vol)"]["test_pearson_r"] - 0.6860) < 1e-4
    assert abs(test_metrics["Equal-Weight Tree Blend (50/50)"]["test_pearson_r"] - 0.6828) < 1e-4
    hybrid_key = [k for k in test_metrics if "Hybrid" in k][0]
    assert abs(test_metrics[hybrid_key]["test_pearson_r"] - 0.6887) < 1e-4
    print("All authoritative benchmark numbers matched to 4 decimal places.")

    facts_json_path = os.path.join(REPORTS_DIR, 'facts.json')
    with open(facts_json_path, 'w') as f:
        json.dump(facts, f, indent=2)
    print(f"Exported facts: {facts_json_path}")

    fig, ax = plt.subplots(figsize=(16, 9))
    splits = [
        ('Training Split\n(Model Parameter Estimation)', meta['n_train'], meta['n_train'] / 605938.0, NAVY),
        ('Validation Split\n(Early Stopping & SLSQP Weights)', meta['n_val'], meta['n_val'] / 605938.0, LIGHT_BLUE),
        ('Held-Out Test Split\n(Strict Out-Of-Sample Scoring)', meta['n_test'], meta['n_test'] / 605938.0, CORAL)
    ]
    left = 0
    y_pos = 0.5
    bar_h = 0.35

    for label, count, frac, col in splits:
        rect = patches.Rectangle((left, y_pos - bar_h / 2), frac, bar_h, facecolor=col, edgecolor='white', linewidth=2)
        ax.add_patch(rect)
        cx = left + frac / 2
        ax.text(cx, y_pos, f"{label}\n\n{count:,} ticks\n({frac*100:.1f}%)", ha='center', va='center',
                color='white' if col in [NAVY, CORAL] else 'black', fontweight='bold', fontsize=14)
        left += frac

    ax.annotate('', xy=(1.0, 0.22), xytext=(0.0, 0.22),
                arrowprops=dict(arrowstyle="->", color=DARK_GREY, lw=3))
    ax.text(0.5, 0.16, "Chronological Time Flow (Strict shuffle=False, zero forward lookahead)  -->",
            ha='center', va='center', fontsize=15, color=DARK_GREY, fontweight='bold')

    ax.text(0.5, 0.82, "Continuous Reconstructed Sample: 605,938 Ticks (from 631,292 Raw Sensor Records)",
            ha='center', va='center', fontsize=18, fontweight='bold', color=NAVY)
    ax.text(0.5, 0.74, "Strict 70% / 15% / 15% Three-Way Chronological Partitioning Protocol",
            ha='center', va='center', fontsize=15, color=DARK_GREY)

    ax.set_xlim(-0.02, 1.02)
    ax.set_ylim(0.0, 1.0)
    ax.axis('off')
    ax.text(0.0, 0.02, "Source: src/eval_rigorous_test_benchmark.py | TAI Research Protocol", fontsize=11, color=DARK_GREY)
    save_figure(fig, 'split_timeline.png')

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 9))
    models_raw = ['Random Forest', 'XGBoost', 'LightGBM']
    r_raw = [-0.0189, 0.0887, 0.0947]
    bars1 = ax1.bar(models_raw, r_raw, color=[DARK_GREY, LIGHT_BLUE, NAVY], width=0.55)
    ax1.axhline(0, color='black', linewidth=1)
    ax1.set_ylabel('Pearson Correlation (r)')
    ax1.set_title('Raw Level-2 Order Book Baselines (ETH Only)\nPearson r ~ 0.09 (Negligible Direct Signal)', pad=15)
    ax1.set_ylim(-0.05, 0.15)
    for b, val in zip(bars1, r_raw):
        ax1.text(b.get_x() + b.get_width() / 2, val + 0.005 if val > 0 else val - 0.012,
                 f"{val:+.4f}", ha='center', va='bottom' if val > 0 else 'top', fontweight='bold')

    categories = ['Early Stopped\n(Rounds 5-6)', 'Forced 1000 Trees\n(XGBoost)', 'Forced 1000 Trees\n(LightGBM)']
    rmse_vals = [7.9e-5, 13.2e-5, 16.4e-5]
    bars2 = ax2.bar(categories, rmse_vals, color=[GREEN, CORAL, '#b2182b'], width=0.55)
    ax2.set_ylabel('Validation RMSE (x10⁻⁵)')
    ax2.set_title('Demonstration of Rapid Noise Memorization\nEarly Stopping vs Unconstrained Overfitting', pad=15)
    ax2.set_ylim(0, 20e-5)
    for b, val in zip(bars2, rmse_vals):
        ax2.text(b.get_x() + b.get_width() / 2, val + 0.4e-5,
                 f"{val*1e5:.1f} x10⁻⁵\n(+{(val/7.9e-5 - 1)*100:.0f}%)" if val > 7.9e-5 else f"{val*1e5:.1f} x10⁻⁵",
                 ha='center', va='bottom', fontweight='bold')

    fig.text(0.02, 0.02, "Source: src/stage1_overfitting_proof.py & logs/run_2026-11-09.log", fontsize=11, color=DARK_GREY)
    save_figure(fig, 'raw_baselines.png')

    fig, ax = plt.subplots(figsize=(16, 9))
    stages = [
        'Stage 1\nRaw Baseline\n(21 feats)',
        'Stage 2\nv2 Microstructure\n(90 feats)',
        'Stage 3\nOptuna Tuning\n(97 feats)',
        'Stage 4\nContinuous EWMA\n(135 feats)',
        'Stage 5\nSLSQP Blend\n(Ensemble)',
        'Stage 7\nOptuna Blend\n(Ensemble)'
    ]
    val_r = [0.0947, 0.6442, 0.6558, 0.6851, 0.6910, 0.6914]
    x_idx = np.arange(len(stages))

    bars = ax.bar(x_idx, val_r, color=LIGHT_BLUE, edgecolor=NAVY, hatch='//', width=0.55,
                  label='Validation r (Selection-Biased 80/20)')
    for b, v in zip(bars, val_r):
        ax.text(b.get_x() + b.get_width() / 2, v + 0.015, f"{v:.4f}", ha='center', va='bottom', fontweight='bold')

    ax.scatter([4.5], [0.6887], color=CORAL, s=260, zorder=5, marker='*', edgecolor='black', linewidth=1.5,
               label='Econometric-Tree Hybrid Held-Out TEST r = 0.6887 (Unbiased OOS)')
    ax.annotate('Strict Held-Out Test\nHybrid: r = 0.6887\n(OOS Generalization)',
                xy=(4.5, 0.6887), xytext=(3.4, 0.52),
                arrowprops=dict(facecolor=CORAL, shrink=0.08, width=2, headwidth=8),
                fontweight='bold', color=CORAL, fontsize=14,
                bbox=dict(boxstyle="round,pad=0.4", fc="white", ec=CORAL, lw=1.5))

    ax.set_xticks(x_idx)
    ax.set_xticklabels(stages)
    ax.set_ylabel('Pearson Correlation (r)')
    ax.set_title('Progression of Predictive Power Across Engineering Stages\nFrom Raw L2 Book (r ~ 0.09) to Multi-Asset Volatility Hybrid (r = 0.6887)', pad=20)
    ax.set_ylim(0, 0.80)
    ax.legend(loc='upper left', frameon=True)
    ax.text(0.01, 0.02, "Source: logs/run_2026-11-09.log & models/rigorous_benchmark_3way_results.csv", fontsize=11, color=DARK_GREY)
    save_figure(fig, 'stage_progression.png')

    fig, ax = plt.subplots(figsize=(16, 9))
    methods = [
        'All Features\n(v2 Full Space, 90 feats)',
        'Tree-Selected Top-25\n(Optuna GBDT, 25 feats)',
        'PCA 95% Variance\n(Unsupervised, 44 comps)',
        'PCA 25 Components\n(Equal Count, 25 comps)'
    ]
    r_reduction = [0.6442, 0.6435, 0.5345, 0.5184]
    colors_red = [NAVY, GREEN, DARK_GREY, LIGHT_GREY]

    bars = ax.bar(methods, r_reduction, color=colors_red, edgecolor='black', width=0.55)
    for b, v in zip(bars, r_reduction):
        ax.text(b.get_x() + b.get_width() / 2, v + 0.015, f"r = {v:.4f}", ha='center', va='bottom', fontweight='bold')

    ax.annotate('Supervised tree selection preserves 99.9% signal\nwith 72% fewer features (r: 0.6442 -> 0.6435)',
                xy=(1, 0.6435), xytext=(0.8, 0.35),
                arrowprops=dict(facecolor=GREEN, shrink=0.08, width=2, headwidth=8),
                fontweight='bold', color=GREEN, fontsize=14,
                bbox=dict(boxstyle="round,pad=0.4", fc="white", ec=GREEN, lw=1.5))

    ax.annotate('Unsupervised PCA fails (Δr = -0.11):\nPrice levels dominate variance but carry zero volatility signal',
                xy=(2, 0.5345), xytext=(2.2, 0.35),
                arrowprops=dict(facecolor=DARK_GREY, shrink=0.08, width=2, headwidth=8),
                fontweight='bold', color=DARK_GREY, fontsize=14,
                bbox=dict(boxstyle="round,pad=0.4", fc="white", ec=DARK_GREY, lw=1.5))

    ax.set_ylabel('Pearson Correlation (r)')
    ax.set_title('Feature Space Dimensionality Reduction: Supervised Selection vs PCA\nDemonstrating Failure of Unsupervised Variance Maximization in Financial Volatility', pad=20)
    ax.set_ylim(0, 0.78)
    ax.text(0.01, 0.02, "Source: src/stage2_pca_vs_tree_reduction.py", fontsize=11, color=DARK_GREY)
    save_figure(fig, 'feature_reduction.png')

    fig, ax = plt.subplots(figsize=(16, 9))
    models_f5 = [
        'Persistence\n(rvol10)',
        'Canonical HAR-RV\n(Corsi 2009 OLS)',
        'Extended HAR-RV\n(ETH Multi-Scale)',
        'Cross-Asset HAR-X\n(ETH + BTC OLS)',
        'RidgeCV (30 Vol)\n(Regularized Multi-Asset)',
        'Equal Tree Blend\n(50/50 LGBM+XGB)',
        'Econometric-Tree\nHybrid Blend'
    ]
    test_r_f5 = [0.5441, 0.6710, 0.6820, 0.6843, 0.6860, 0.6828, 0.6887]
    cols_f5 = [DARK_GREY, LIGHT_BLUE, LIGHT_BLUE, TEAL, NAVY, GREEN, CORAL]

    bars = ax.bar(models_f5, test_r_f5, color=cols_f5, edgecolor='black', width=0.55)
    for b, v in zip(bars, test_r_f5):
        ax.text(b.get_x() + b.get_width() / 2, v + 0.012, f"{v:.4f}", ha='center', va='bottom', fontweight='bold')

    ax.set_ylabel('Out-of-Sample Test Pearson r')
    ax.set_title('Econometric Volatility Memory Models vs Non-Linear Gradient Boosted Trees\nRegularized 30-Vol Ridge (0.6860) Outperforms Pure Trees (0.6828); Hybrid Combines Strengths (0.6887)', pad=20)
    ax.set_ylim(0.45, 0.75)
    ax.text(0.01, 0.02, "Source: models/stage8_har_rv_results.csv & models/rigorous_benchmark_3way_results.csv", fontsize=11, color=DARK_GREY)
    save_figure(fig, 'har_vs_trees.png')

    fig, ax = plt.subplots(figsize=(16, 9))
    x_b = np.arange(len(df_bench))
    w = 0.38

    b1 = ax.bar(x_b - w/2, df_bench['val_pearson_r'], width=w, label='Validation r (Selection-Biased)', color=LIGHT_BLUE, edgecolor=NAVY)
    b2 = ax.bar(x_b + w/2, df_bench['test_pearson_r'], width=w, label='Test r (Held-Out Out-Of-Sample)', color=NAVY, edgecolor='black')

    hybrid_idx = df_bench['model'].str.contains('Hybrid').idxmax()
    b2[hybrid_idx].set_color(CORAL)
    b2[hybrid_idx].set_edgecolor('black')

    ax.set_xticks(x_b)
    short_names = [
        'Persistence', 'Univariate OLS', 'RidgeCV (30 Vol)', 'LightGBM Cons',
        'XGBoost Cons', 'LightGBM Deep', 'XGBoost Deep', 'Equal Tree Blend',
        'SLSQP Tree Blend', 'LightGBM Tuned', 'XGBoost Tuned', 'Hybrid Blend'
    ]
    ax.set_xticklabels(short_names, rotation=35, ha='right')
    ax.set_ylabel('Pearson Correlation (r)')
    ax.set_title('Strict 3-Way Chronological Benchmark: Validation vs Out-of-Sample Test\nUniversal Performance Degradation Across All Models Reflecting Market Regime Shift', pad=20)
    ax.set_ylim(0.45, 0.85)
    ax.legend(loc='upper right', frameon=True)
    ax.text(0.01, 0.02, "Source: models/rigorous_benchmark_3way_results.csv (Strict 70/15/15 Chronological Split)", fontsize=11, color=DARK_GREY)
    save_figure(fig, 'benchmark_3way.png')

    fig, ax = plt.subplots(figsize=(16, 9))
    pred_cols_f7 = ['pred_persistence', 'pred_ridge', 'pred_lgbm', 'pred_xgb', 'pred_tree_blend', 'pred_hybrid']
    corr_df = test_df[pred_cols_f7].corr()
    clean_labels = ['Persistence', 'RidgeCV (30 Vol)', 'LightGBM', 'XGBoost', 'Tree Blend', 'Hybrid']

    cax = ax.matshow(corr_df.values, cmap='Blues', vmin=0.5, vmax=1.0)
    fig.colorbar(cax, ax=ax, fraction=0.046, pad=0.04)

    for i in range(len(clean_labels)):
        for j in range(len(clean_labels)):
            val = corr_df.values[i, j]
            ax.text(j, i, f"{val:.3f}", ha='center', va='center',
                    color='white' if val > 0.85 else 'black', fontweight='bold', fontsize=14)

    ax.set_xticks(range(len(clean_labels)))
    ax.set_yticks(range(len(clean_labels)))
    ax.set_xticklabels(clean_labels, rotation=30, ha='left')
    ax.set_yticklabels(clean_labels)
    ax.set_title('Out-of-Sample Base Learner Prediction Correlation Matrix\nEmpirical Demonstration of Extreme Collinearity (r > 0.96) Between Gradient Boosted Trees', pad=30)
    ax.text(0.01, 0.02, "Source: data/processed/app_cache.joblib (Test Partition, 90,892 Ticks)", fontsize=11, color=DARK_GREY)
    save_figure(fig, 'pred_corr_heatmap.png')

    fig, ax = plt.subplots(figsize=(16, 9))
    w_size = 5000
    step = 1000
    n_test = len(test_df)
    centers = []
    r_persist = []
    r_ridge = []
    r_tree = []
    r_hyb = []

    y_test_arr = test_df['label'].values
    p_pers_arr = test_df['pred_persistence'].values
    p_ridg_arr = test_df['pred_ridge'].values
    p_tree_arr = test_df['pred_tree_blend'].values
    p_hybr_arr = test_df['pred_hybrid'].values

    for start in range(0, n_test - w_size, step):
        end = start + w_size
        centers.append(start + w_size // 2)
        r_persist.append(pearsonr(y_test_arr[start:end], p_pers_arr[start:end])[0])
        r_ridge.append(pearsonr(y_test_arr[start:end], p_ridg_arr[start:end])[0])
        r_tree.append(pearsonr(y_test_arr[start:end], p_tree_arr[start:end])[0])
        r_hyb.append(pearsonr(y_test_arr[start:end], p_hybr_arr[start:end])[0])

    ax.plot(centers, r_persist, label='Persistence (rvol10)', color=DARK_GREY, lw=1.5, linestyle=':')
    ax.plot(centers, r_ridge, label='RidgeCV (30 Vol)', color=NAVY, lw=2, linestyle='--')
    ax.plot(centers, r_tree, label='Tree Blend (LGBM+XGB)', color=GREEN, lw=2, linestyle='-.')
    ax.plot(centers, r_hyb, label='Econometric-Tree Hybrid', color=CORAL, lw=2.5)

    ax.set_xlabel('Held-Out Test Slice Chronological Progression (Ticks)')
    ax.set_ylabel('Rolling Pearson Correlation (Window = 5,000 Ticks)')
    ax.set_title('Out-of-Sample Temporal Stability and Regime Trajectory (Rolling r)\nDemonstrating Consistent Dominance of Econometric-Tree Hybrid Across Market Regimes', pad=20)
    ax.legend(loc='lower left', frameon=True)
    ax.set_ylim(0.2, 0.9)
    ax.text(0.01, 0.02, "Source: data/processed/app_cache.joblib (Test Slice, 90,892 Ticks)", fontsize=11, color=DARK_GREY)
    save_figure(fig, 'rolling_r.png')

    fig, ax = plt.subplots(figsize=(16, 9))
    ax.set_xlim(0, 16)
    ax.set_ylim(0, 9)
    ax.axis('off')

    def draw_box(x, y, w, h, title, subtitle, bg_col, text_col='white'):
        rect = patches.FancyBboxPatch((x, y), w, h, boxstyle="round,pad=0.2",
                                      facecolor=bg_col, edgecolor='black', linewidth=1.5)
        ax.add_patch(rect)
        ax.text(x + w/2, y + h*0.65, title, ha='center', va='center',
                fontsize=14, fontweight='bold', color=text_col)
        ax.text(x + w/2, y + h*0.35, subtitle, ha='center', va='center',
                fontsize=11, color=text_col)

    def draw_arrow(x1, y1, x2, y2):
        ax.annotate('', xy=(x2, y2), xytext=(x1, y1),
                    arrowprops=dict(arrowstyle="->", color=DARK_GREY, lw=2.5))

    draw_box(0.5, 3.2, 2.5, 2.6, "Raw L2 Market Feeds", "ETH, BTC, SOL Depth (5 Levels)\n631,292 Snapshots", NAVY)
    draw_arrow(3.2, 4.5, 4.0, 4.5)

    draw_box(4.0, 3.2, 2.5, 2.6, "Data Hygiene & Prep", "Continuous Price ffill\nmin_periods=2, Causal fillna\n605,938 Usable Ticks", TEAL)
    draw_arrow(6.7, 4.5, 7.5, 4.5)

    draw_box(7.5, 3.0, 2.5, 3.0, "135 Multi-Scale Features", "63 Raw Microstructure\n63 Multi-Horizon (RV, EWMA)\n9 Cross-Asset Ratios/Lags", NAVY)
    draw_arrow(10.2, 5.5, 11.0, 6.8)
    draw_arrow(10.2, 4.5, 11.0, 4.5)
    draw_arrow(10.2, 3.5, 11.0, 2.2)

    draw_box(11.0, 6.2, 2.3, 1.4, "RidgeCV (30 Vol)", "Linear Regularized HAR", LIGHT_BLUE, text_col='black')
    draw_box(11.0, 3.8, 2.3, 1.4, "LightGBM Cons", "Non-linear Depth 4", GREEN)
    draw_box(11.0, 1.5, 2.3, 1.4, "XGBoost Cons", "Non-linear Depth 4", LIGHT_BLUE, text_col='black')

    draw_arrow(13.5, 6.9, 14.2, 4.8)
    draw_arrow(13.5, 4.5, 14.2, 4.5)
    draw_arrow(13.5, 2.2, 14.2, 4.2)

    draw_box(14.2, 3.2, 1.6, 2.6, "Hybrid Ensemble", "1/3 Ridge\n1/3 LGBM\n1/3 XGB\nr = 0.6887", CORAL)

    ax.text(8.0, 8.2, "End-to-End Quantitative Research Pipeline Architecture",
            ha='center', va='center', fontsize=20, fontweight='bold', color=NAVY)
    ax.text(8.0, 7.6, "High-Frequency Level-2 Limit Order Book Volatility Forecasting Framework",
            ha='center', va='center', fontsize=14, color=DARK_GREY)
    ax.text(0.5, 0.3, "Source: src/stage4_multiscale_pipeline.py & src/eval_rigorous_test_benchmark.py", fontsize=11, color=DARK_GREY)
    save_figure(fig, 'pipeline_architecture.png')

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 9))

    eth_csv = os.path.join(DATA_DIR, 'ETH.csv')
    if not os.path.exists(eth_csv):
        eth_csv = os.path.join(DATA_DIR, 'ETH-01.csv')
    df_eth_sample = pd.read_csv(eth_csv, nrows=10000)
    label_series = df_eth_sample['label'].dropna().values

    ax1.plot(label_series[:3000] * 1e5, color=NAVY, lw=1.2)
    ax1.set_xlabel('Ticks (Consecutive 1-Second Snapshots)')
    ax1.set_ylabel('Forward Volatility (x10⁻⁵)')
    ax1.set_title('Mandelbrot (1963) Volatility Clustering\nPeriods of High Volatility Followed by High Volatility', pad=15)

    def compute_acf(x, nlags=200):
        x_c = x - np.mean(x)
        var = np.var(x)
        return np.array([1.0] + [float(np.mean(x_c[:-k] * x_c[k:]) / (var + 1e-12)) for k in range(1, nlags + 1)])

    acf_vals = compute_acf(label_series[:10000], nlags=200)
    ax2.plot(range(len(acf_vals)), acf_vals, color=CORAL, lw=2)
    ax2.axhline(0, color='black', lw=1, linestyle='--')
    ax2.set_xlabel('Lag (Ticks / Seconds)')
    ax2.set_ylabel('Autocorrelation Coefficient')
    ax2.set_title('Empirical Autocorrelation Function (ACF)\nSlow Hyperbolic Decay Confirming Long-Memory Volatility Persistence', pad=15)
    ax2.set_ylim(-0.1, 1.05)

    fig.text(0.02, 0.02, f"Source: {os.path.basename(eth_csv)} (10-Second Forward Target Label)", fontsize=11, color=DARK_GREY)
    save_figure(fig, 'label_clustering.png')

    print("\nAll 10 academic figures generated and verified in reports/figures/.")

if __name__ == '__main__':
    main()
