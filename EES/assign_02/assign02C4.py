import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
import numpy as np
from datetime import datetime
import os

# Sample data from the provided table
data = {
    'Year': [2000, 2005, 2010, 2015, 2020],
    'Deforestation_Rate(Mha/yr)': [2.5, 3.0, 3.8, 4.2, 4.8],
    'Carbon_Released_Deforestation(Gt/yr)': [0.4, 0.5, 0.6, 0.7, 0.8],
    'Fossil_Fuel_Emissions(Gt/yr)': [0.6, 0.8, 1.0, 1.3, 1.5],
    'Total_Carbon_Emissions(Gt/yr)': [1.0, 1.3, 1.6, 2.0, 2.3],
    'Net_Carbon_Uptake(Gt/yr)': [1.2, 1.1, 0.9, 0.8, 0.6]
}

df = pd.DataFrame(data)

def create_monthly_data(df):
    """
    Convert annual data to monthly with seasonal adjustments
    """
    monthly_data = []
    
    # Seasonal adjustment factors (higher deforestation in dry season)
    seasonal_factors = {
        'Deforestation_Rate(Mha/yr)': [0.6, 0.6, 0.7, 0.8, 0.9, 1.0, 1.8, 1.9, 1.7, 0.9, 0.7, 0.6],
        'Carbon_Released_Deforestation(Gt/yr)': [0.6, 0.6, 0.7, 0.8, 0.9, 1.0, 1.8, 1.9, 1.7, 0.9, 0.7, 0.6],
        'Fossil_Fuel_Emissions(Gt/yr)': [1.0, 1.0, 1.0, 0.9, 0.9, 0.9, 1.1, 1.1, 1.1, 1.0, 1.0, 1.0],
        'Net_Carbon_Uptake(Gt/yr)': [1.2, 1.3, 1.2, 1.1, 0.9, 0.8, 0.7, 0.6, 0.7, 0.9, 1.1, 1.2]
    }
    
    for year in df['Year']:
        year_data = df[df['Year'] == year].iloc[0]
        for month in range(12):
            monthly_row = {'Year': year, 'Month': month + 1}
            
            for column in df.columns:
                if column in seasonal_factors:
                    monthly_row[column] = year_data[column] * seasonal_factors[column][month] / 12
                elif column != 'Year':
                    monthly_row[column] = year_data[column] / 12
                    
            monthly_data.append(monthly_row)
    
    return pd.DataFrame(monthly_data)

def create_detailed_plots(df, monthly_df):
    """
    Create and save detailed plots for both annual and monthly data
    """
    if not os.path.exists('output'):
        os.makedirs('output')
    
    # Annual trends
    parameters = {col: col.replace('_', ' ') for col in df.columns if col != 'Year'}
    
    for param, label in parameters.items():
        plt.figure(figsize=(12, 6))
        plt.subplot(1, 2, 1)
        plt.plot(df['Year'], df[param], 'o-', linewidth=2)
        plt.title(f'Annual {label}')
        plt.xlabel('Year')
        plt.ylabel(label)
        plt.grid(True)
        
        # Monthly variation (last year)
        plt.subplot(1, 2, 2)
        last_year_data = monthly_df[monthly_df['Year'] == df['Year'].max()]
        plt.plot(last_year_data['Month'], last_year_data[param], 'o-', linewidth=2)
        plt.title(f'Monthly {label} ({df["Year"].max()})')
        plt.xlabel('Month')
        plt.ylabel(label)
        plt.grid(True)
        
        plt.tight_layout()
        plt.savefig(f'output/{param}_trends.png', dpi=300, bbox_inches='tight')
        plt.close()

def calculate_supply_days(monthly_df):
    """
    Calculate days where carbon uptake exceeds emissions
    """
    supply_days = {}
    
    for year in monthly_df['Year'].unique():
        year_data = monthly_df[monthly_df['Year'] == year]
        days_in_month = pd.Series([31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31])
        
        # Calculate daily surplus/deficit
        daily_balance = year_data['Net_Carbon_Uptake(Gt/yr)'] > year_data['Total_Carbon_Emissions(Gt/yr)']
        supply_days[year] = (daily_balance * days_in_month).sum()
    
    return supply_days

def perform_statistical_analysis(df, monthly_df):
    """
    Perform comprehensive statistical analysis on both annual and monthly data
    """
    stats_results = {
        'Annual': {},
        'Monthly': {},
        'Supply_Days': calculate_supply_days(monthly_df)
    }
    
    # Annual statistics
    for column in df.columns:
        if column != 'Year':
            stats_results['Annual'][column] = {
                'mean': df[column].mean(),
                'std': df[column].std(),
                'min': df[column].min(),
                'max': df[column].max(),
                'trend': df[column].iloc[-1] - df[column].iloc[0]
            }
    
    # Monthly statistics
    for column in monthly_df.columns:
        if column not in ['Year', 'Month']:
            stats_results['Monthly'][column] = {
                'mean': monthly_df[column].mean(),
                'std': monthly_df[column].std(),
                'seasonal_variation': monthly_df.groupby('Month')[column].mean().std()
            }
    
    return stats_results

def make_predictions(df):
    """
    Make predictions for the next year using linear regression
    """
    next_year = df['Year'].max() + 1
    predictions = {}
    
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
    
    return predictions

def analyze_mitigation_strategies():
    """
    Analyze the impact of different mitigation strategies
    """
    strategies = {
        'Reforestation': {
            'carbon_sequestration_rate': 0.5,  # Gt CO2/yr per Mha
            'implementation_cost': 'Medium',
            'time_to_effect': '5-10 years',
            'co_benefits': ['Biodiversity', 'Soil conservation', 'Water regulation']
        },
        'Emission_Reduction': {
            'potential_reduction': 0.3,  # 30% reduction
            'implementation_cost': 'High',
            'time_to_effect': '1-3 years',
            'co_benefits': ['Air quality', 'Public health', 'Energy efficiency']
        }
    }
    
    return strategies

def main():
    print("Starting Amazon Carbon Cycle Analysis...")
    
    try:
        # Create monthly data
        monthly_df = create_monthly_data(df)
        
        # Create visualizations
        create_detailed_plots(df, monthly_df)
        
        # Perform statistical analysis
        stats = perform_statistical_analysis(df, monthly_df)
        
        # Make predictions
        predictions = make_predictions(df)
        
        # Analyze mitigation strategies
        strategies = analyze_mitigation_strategies()
        
        # Print summary results
        print("\nANALYSIS COMPLETE")
        print("-" * 50)
        print("\nSupply Days Summary:")
        for year, days in stats['Supply_Days'].items():
            print(f"{year}: {int(days)} days")
        
        print("\nPredictions for next year:")
        for param, values in predictions.items():
            print(f"{param}: {values['predicted_value']:.2f} (R² = {values['r_squared']:.3f})")
        
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        print("Please check your data and try again.")

if __name__ == "__main__":
    main()