import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
import numpy as np
from datetime import datetime
import os

# Create comprehensive dataset
data = {
    'Year': [2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021],
    'Total_Rainfall(mm)': [935, 880, 920, 865, 845, 830, 950, 1020, 890, 970],
    'Evaporation_Rate(%)': [53, 56, 54, 58, 60, 62, 45, 57, 40, 55],
    'Precipitation_Rate(%)': [28, 24, 26, 23, 22, 20, 27, 30, 23, 28],
    'Runoff_Rate(%)': [36, 38, 37, 30, 40, 43, 35, 30, 30, 36],
    'Temperature(C)': [29.8, 30, 29.7, 30.2, 30.5, 30.8, 28.5, 28, 29.5, 28.7],
    'Urbanization_Rate(%)': [55, 58, 60, 62, 64, 65, 67, 69, 71, 73]
}

df = pd.DataFrame(data)

def create_detailed_plots():
    """Create detailed visualizations of all parameters"""
    sns.set_theme(style="whitegrid")  # Use Seaborn's theme


    fig, axes = plt.subplots(2, 3, figsize=(18, 12))
    axes = axes.flatten()
    
    parameters = {
        'Total_Rainfall(mm)': 'Rainfall (mm)',
        'Evaporation_Rate(%)': 'Evaporation (%)',
        'Precipitation_Rate(%)': 'Precipitation (%)',
        'Runoff_Rate(%)': 'Runoff (%)',
        'Temperature(C)': 'Temperature (C)',
        'Urbanization_Rate(%)': 'Urbanization (%)'
    }
    
    for idx, (param, label) in enumerate(parameters.items()):
        axes[idx].plot(df['Year'], df[param], 'o-', linewidth=2)
        axes[idx].set_title(f'{param.replace("_", " ")}')
        axes[idx].set_xlabel('Year')
        axes[idx].set_ylabel(label)
        axes[idx].grid(True, linestyle='--', alpha=0.7)
        
        for x, y in zip(df['Year'], df[param]):
            axes[idx].annotate(f'{y:.1f}', 
                             (x, y), 
                             textcoords="offset points", 
                             xytext=(0,10), 
                             ha='center')
    
    plt.tight_layout()
    plt.savefig('output/water_cycle_analysis.png', dpi=300, bbox_inches='tight')
    plt.close()

def perform_statistical_analysis():
    """Perform comprehensive statistical analysis"""
    stats_results = {}
    
    for column in df.columns:
        if column != 'Year':
            stats = {
                'mean': df[column].mean(),
                'std': df[column].std(),
                'min': df[column].min(),
                'max': df[column].max(),
                'trend': df[column].iloc[-1] - df[column].iloc[0]
            }
            stats_results[column] = stats
    
    print("\nSTATISTICAL ANALYSIS")
    print("-" * 50)
    for param, stats in stats_results.items():
        print(f"\n{param.replace('_', ' ')}:")
        print(f"  Average: {stats['mean']:.2f}")
        print(f"  Standard Deviation: {stats['std']:.2f}")
        print(f"  Range: {stats['min']:.2f} to {stats['max']:.2f}")
        print(f"  Overall Change: {stats['trend']:.2f}")
    
    return stats_results

def make_advanced_predictions():
    """Make predictions with confidence intervals"""
    next_year = df['Year'].max() + 1
    predictions = {}
    
    print("\nPREDICTIONS FOR", next_year)
    print("-" * 50)
    
    for column in df.columns:
        if column != 'Year':
            X = df['Year'].values.reshape(-1, 1)
            y = df[column].values
            
            model = LinearRegression()
            model.fit(X, y)
            
            prediction = model.predict([[next_year]])[0]
            r_squared = model.score(X, y)
            
            predictions[column] = {
                'predicted_value': prediction,
                'r_squared': r_squared
            }
            
            print(f"\n{column.replace('_', ' ')}:")
            print(f"  Predicted Value: {prediction:.2f}")
            print(f"  Confidence (R²): {r_squared:.3f}")
    
    return predictions

def analyze_interventions():
    """Analyze effectiveness of government interventions"""
    print("\nANALYSIS OF GOVERNMENT INITIATIVES")
    print("-" * 50)
    
    avg_rainfall = df['Total_Rainfall(mm)'].mean()
    runoff_trend = df['Runoff_Rate(%)'].iloc[-1] - df['Runoff_Rate(%)'].iloc[0]
    potential_savings = avg_rainfall * (runoff_trend / 100)
    
    print("\nRainwater Harvesting Analysis:")
    print(f"  Average Annual Rainfall: {avg_rainfall:.1f} mm")
    print(f"  Change in Runoff Rate: {runoff_trend:.1f}%")
    print(f"  Potential Water Savings: {potential_savings:.1f} mm per year")
    
    urbanization_change = df['Urbanization_Rate(%)'].iloc[-1] - df['Urbanization_Rate'].iloc[0]
    temp_change = df['Temperature(C)'].iloc[-1] - df['Temperature'].iloc[0]
    
    print("\nUrbanization Impact:")
    print(f"  Urbanization Rate Change: {urbanization_change:.1f}%")
    print(f"  Temperature Change: {temp_change:.1f}°C")
    
    print("\nRecommendations:")
    if runoff_trend > 0:
        print("  • Implement more rainwater harvesting systems")
    if urbanization_change > 0:
        print("  • Increase green roof coverage")
    if temp_change > 0:
        print("  • Enhance urban forestry programs")

def generate_report(stats_results, predictions):
    """Generate comprehensive analysis report"""
    report_text = """
Water Cycle Disruption Analysis Report
Generated on: {date}

1. INTRODUCTION
---------------
This report analyzes the water cycle disruption in urban areas, focusing on key parameters
including rainfall, evaporation, precipitation, runoff, temperature, and urbanization rates.

2. METHODOLOGY
-------------
Data was analyzed using Python, employing:
- Pandas for data manipulation
- Matplotlib for visualization
- Scikit-learn for predictive modeling

3. RESULTS
----------
3.1 Statistical Analysis:
""".format(date=datetime.now().strftime("%Y-%m-%d"))

    for param, stats in stats_results.items():
        report_text += f"\n{param.replace('_', ' ')}:\n"
        report_text += f"  • Average: {stats['mean']:.2f}\n"
        report_text += f"  • Range: {stats['min']:.2f} to {stats['max']:.2f}\n"
        report_text += f"  • Overall Trend: {stats['trend']:.2f}\n"

    report_text += "\n3.2 Predictions for Next Year:\n"
    for param, pred in predictions.items():
        report_text += f"  • {param.replace('_', ' ')}: {pred['predicted_value']:.2f}\n"
        report_text += f"    Confidence (R²): {pred['r_squared']:.3f}\n"

    with open('output/WaterCycleDisruptionReport.txt', 'w') as f:
        f.write(report_text)

    return report_text


def create_interactive_analysis():
    """Create interactive menu for analysis"""
    while True:
        print("\nWater Cycle Analysis Menu:")
        print("1. View Basic Statistics")
        print("2. Show Trends Graph")
        print("3. View Predictions")
        print("4. Analyze Government Initiatives")
        print("5. Generate Complete Report")
        print("6. Exit")
        
        choice = input("\nEnter your choice (1-6): ")
        
        if choice == '1':
            perform_statistical_analysis()
        elif choice == '2':
            create_detailed_plots()
            print("Graphs saved in output folder")
        elif choice == '3':
            make_advanced_predictions()
        elif choice == '4':
            analyze_interventions()
        elif choice == '5':
            stats = perform_statistical_analysis()
            preds = make_advanced_predictions()
            generate_report(stats, preds)
            print("\nReport generated successfully in output folder!")
        elif choice == '6':
            break
        else:
            print("Invalid choice. Please try again.")

def main():
    """Main function to run the analysis"""
    print("Starting Water Cycle Disruption Analysis...")
    
    # Create output directory
    if not os.path.exists('output'):
        os.makedirs('output')
    

if __name__ == "__main__":
    main()