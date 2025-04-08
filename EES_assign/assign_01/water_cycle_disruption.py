import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
import numpy as np
from datetime import datetime
import os

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
    sns.set_theme(style="whitegrid")  

    parameters = {
        'Total_Rainfall(mm)': 'Rainfall (mm)',
        'Evaporation_Rate(%)': 'Evaporation (%)',
        'Precipitation_Rate(%)': 'Precipitation (%)',
        'Runoff_Rate(%)': 'Runoff (%)',
        'Temperature(C)': 'Temperature (C)',
        'Urbanization_Rate(%)': 'Urbanization (%)'
    }
    
    for param, label in parameters.items():
        plt.figure(figsize=(10, 6))
        plt.plot(df['Year'], df[param], 'o-', linewidth=2)
        plt.title(f'{param.replace("_", " ")}')
        plt.xlabel('Year')
        plt.ylabel(label)
        plt.grid(True, linestyle='--', alpha=0.7)
        
        for x, y in zip(df['Year'], df[param]):
            plt.annotate(f'{y:.1f}', 
                         (x, y), 
                         textcoords="offset points", 
                         xytext=(0,10), 
                         ha='center')
        
        plt.tight_layout()
        plt.savefig(f'output/{param}.png', dpi=300, bbox_inches='tight')
        plt.close()

def perform_statistical_analysis():
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
    results = {}
    print("\nANALYSIS OF GOVERNMENT INITIATIVES")
    print("-" * 50)
    
    avg_rainfall = df['Total_Rainfall(mm)'].mean()
    runoff_trend = df['Runoff_Rate(%)'].iloc[-1] - df['Runoff_Rate(%)'].iloc[0]
    potential_savings = avg_rainfall * (runoff_trend / 100)
    
    results['Rainwater Harvesting Analysis'] = {
        'Average Annual Rainfall': f"{avg_rainfall:.1f} mm",
        'Change in Runoff Rate': f"{runoff_trend:.1f}%",
        'Potential Water Savings': f"{potential_savings:.1f} mm per year"
    }
    
    urbanization_change = df['Urbanization_Rate(%)'].iloc[-1] - df['Urbanization_Rate(%)'].iloc[0]
    temp_change = df['Temperature(C)'].iloc[-1] - df['Temperature(C)'].iloc[0]
    
    results['Urbanization Impact'] = {
        'Urbanization Rate Change': f"{urbanization_change:.1f}%",
        'Temperature Change': f"{temp_change:.1f}°C"
    }
    
    recommendations = []
    if runoff_trend > 0:
        recommendations.append("Implement more rainwater harvesting systems")
    if urbanization_change > 0:
        recommendations.append("Increase green roof coverage")
    if temp_change > 0:
        recommendations.append("Enhance urban forestry programs")
    
    results['Recommendations'] = recommendations  
    
    return results
    
def safe_analysis():
    """Validate data and check for errors"""
    try:
        if df.empty:
            raise ValueError("No data found in the DataFrame")
            
        if df.isnull().any().any():
            print("Warning: Data contains missing values")
            df.fillna(df.mean(), inplace=True)
            
        for column in ['Total_Rainfall(mm)', 'Temperature(C)']:
            if (df[column] < 0).any():
                raise ValueError(f"Invalid negative values found in {column}")
                
        for column in ['Evaporation_Rate(%)', 'Precipitation_Rate(%)', 'Runoff_Rate(%)']:
            if (df[column] > 100).any():
                raise ValueError(f"Invalid percentage values found in {column}")
                
        return True
        
    except Exception as e:
        print(f"Error in data validation: {str(e)}")
        return False

def create_interactive_analysis():
    print("Starting Interactive Analysis...")
    while True:
        print("\nWater Cycle Analysis Menu:")
        print("1. View Basic Statistics")
        print("2. Show Trends Graph")
        print("3. View Predictions")
        print("4. Analyze Government Initiatives")
        print("5. Exit")
        
        choice = input("\nEnter your choice (1-5): ")
        
        if choice == '1':
            perform_statistical_analysis()
        elif choice == '2':
            create_detailed_plots()
            print("Graphs saved in output folder")
        elif choice == '3':
            make_advanced_predictions()
        elif choice == '4':
            interventions_results = analyze_interventions()
            for section, results in interventions_results.items():
                print(f"\n{section}:")
                if isinstance(results, list):  
                    for recommendation in results:
                        print(f"  • {recommendation}")
                else:
                    for key, value in results.items():
                        print(f"  • {key}: {value}")
        elif choice == '5':
            break
        else:
            print("Invalid choice. Please try again.")

def main():
    """Main function to run the analysis"""
    print("Starting Water Cycle Disruption Analysis...")
    
    if not os.path.exists('output'):
        os.makedirs('output')

    # Validate data
    if not safe_analysis():
        print("Error in data validation. Please check your data.")
        return
    
    try:
        # Run interactive analysis
        create_interactive_analysis()
        
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        print("Please check your data and try again.")
        
if __name__ == "__main__":
    main()