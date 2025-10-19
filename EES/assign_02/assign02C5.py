import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
import numpy as np
from datetime import datetime
import os

# Updated data from 1970 to 2020
data = {
    'Year': [1970, 1975, 1980, 1985, 1990, 1995, 2000, 2005, 2010, 2015, 2020],
    'Deforestation_Rate(Mha/yr)': [0.8, 1.2, 1.6, 2.0, 2.2, 2.4, 2.5, 3.0, 3.8, 4.2, 4.8],
    'Carbon_Released_Deforestation(Gt/yr)': [0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.5, 0.6, 0.7, 0.8],
    'Fossil_Fuel_Emissions(Gt/yr)': [0.2, 0.3, 0.4, 0.5, 0.55, 0.6, 0.6, 0.8, 1.0, 1.3, 1.5],
    'Total_Carbon_Emissions(Gt/yr)': [0.3, 0.45, 0.6, 0.75, 0.85, 0.95, 1.0, 1.3, 1.6, 2.0, 2.3],
    'Net_Carbon_Uptake(Gt/yr)': [2.0, 1.9, 1.8, 1.7, 1.6, 1.5, 1.2, 1.1, 0.9, 0.8, 0.6]
}

df = pd.DataFrame(data)

def create_monthly_data(df):
    """
    Convert annual data to monthly with seasonal adjustments
    """
    monthly_data = []
    
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
    
    parameters = {col: col.replace('_', ' ') for col in df.columns if col != 'Year'}
    
    for param, label in parameters.items():
        plt.figure(figsize=(12, 6))
        plt.subplot(1, 2, 1)
        plt.plot(df['Year'], df[param], 'o-', linewidth=2)
        plt.title(f'Annual {label}')
        plt.xlabel('Year')
        plt.ylabel(label)
        plt.grid(True)
        
        plt.subplot(1, 2, 2)
        last_year_data = monthly_df[monthly_df['Year'] == df['Year'].max()]
        plt.plot(last_year_data['Month'], last_year_data[param], 'o-', linewidth=2)
        plt.title(f'Monthly {label} ({df["Year"].max()})')
        plt.xlabel('Month')
        plt.ylabel(label)
        plt.grid(True)
        
        plt.tight_layout()
        # Fix: Replace '/' in filenames
        safe_param = param.replace('/', '_')
        plt.savefig(f'output/{safe_param}_trends.png', dpi=300, bbox_inches='tight')
        plt.close()

    print("\nPlots have been saved in the 'output' directory.")

def calculate_supply_days(monthly_df):
    """
    Calculate days where carbon uptake exceeds emissions
    """
    supply_days = {}
    
    for year in monthly_df['Year'].unique():
        year_data = monthly_df[monthly_df['Year'] == year]
        days_in_month = pd.Series([31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31])
        
        daily_balance = year_data['Net_Carbon_Uptake(Gt/yr)'] > year_data['Total_Carbon_Emissions(Gt/yr)']
        supply_days[year] = (daily_balance * days_in_month).sum()
    
    return supply_days

def perform_statistical_analysis(df, monthly_df):
    """
    Perform comprehensive statistical analysis on both annual and monthly data
    """
    print("\nSTATISTICAL ANALYSIS")
    print("-" * 50)
    
    for column in df.columns:
        if column != 'Year':
            print(f"\n{column.replace('_', ' ')}:")
            print(f"  Annual Average: {df[column].mean():.2f}")
            print(f"  Standard Deviation: {df[column].std():.2f}")
            print(f"  Range: {df[column].min():.2f} to {df[column].max():.2f}")
            print(f"  Overall Change: {df[column].iloc[-1] - df[column].iloc[0]:.2f}")
            
            # Monthly patterns
            monthly_stats = monthly_df.groupby('Month')[column].mean()
            print(f"  Peak Month: {monthly_stats.idxmax()} (Value: {monthly_stats.max():.2f})")
            print(f"  Low Month: {monthly_stats.idxmin()} (Value: {monthly_stats.min():.2f})")

def make_predictions(df):
    """
    Make predictions for the next year using linear regression
    """
    next_year = df['Year'].max() + 1
    print(f"\nPREDICTIONS FOR {next_year}")
    print("-" * 50)
    
    for column in df.columns:
        if column != 'Year':
            X = df['Year'].values.reshape(-1, 1)
            y = df[column].values
            
            model = LinearRegression()
            model.fit(X, y)
            
            prediction = model.predict([[next_year]])[0]
            r_squared = model.score(X, y)
            
            print(f"\n{column.replace('_', ' ')}:")
            print(f"  Predicted Value: {prediction:.2f}")
            print(f"  Model Confidence (R²): {r_squared:.3f}")

def analyze_mitigation_strategies():
    """
    Analyze and display information about mitigation strategies
    """
    print("\nMITIGATION STRATEGIES ANALYSIS")
    print("-" * 50)
    
    strategies = {
        'Reforestation': {
            'Carbon Impact': '0.5 Gt CO₂/yr per Mha',
            'Implementation Cost': 'Medium',
            'Time to Effect': '5-10 years',
            'Co-benefits': ['Biodiversity', 'Soil conservation', 'Water regulation']
        },
        'Emission Reduction': {
            'Carbon Impact': '30% potential reduction',
            'Implementation Cost': 'High',
            'Time to Effect': '1-3 years',
            'Co-benefits': ['Air quality', 'Public health', 'Energy efficiency']
        },
        'Conservation': {
            'Carbon Impact': 'Prevents 0.8 Gt CO₂/yr loss',
            'Implementation Cost': 'Medium-High',
            'Time to Effect': 'Immediate',
            'Co-benefits': ['Ecosystem preservation', 'Indigenous rights', 'Climate stability']
        }
    }
    
    for strategy, details in strategies.items():
        print(f"\n{strategy}:")
        for key, value in details.items():
            if isinstance(value, list):
                print(f"  {key}: {', '.join(value)}")
            else:
                print(f"  {key}: {value}")

def create_interactive_analysis():
    """
    Provide an interactive menu for data analysis
    """
    monthly_df = create_monthly_data(df)
    
    while True:
        print("\nAmazon Carbon Cycle Analysis Menu:")
        print("1. View Basic Statistics")
        print("2. Show Trends Graphs")
        print("3. View Future Predictions")
        print("4. Calculate Supply Days")
        print("5. Analyze Mitigation Strategies")
        print("6. Exit")
        
        choice = input("\nEnter your choice (1-6): ")
        
        if choice == '1':
            perform_statistical_analysis(df, monthly_df)
        elif choice == '2':
            create_detailed_plots(df, monthly_df)
        elif choice == '3':
            make_predictions(df)
        elif choice == '4':
            supply_days = calculate_supply_days(monthly_df)
            print("\nDAYS OF POSITIVE CARBON BALANCE")
            print("-" * 50)
            for year, days in supply_days.items():
                print(f"{year}: {int(days)} days")
        elif choice == '5':
            analyze_mitigation_strategies()
        elif choice == '6':
            print("\nExiting program. Thank you for using the Carbon Cycle Analysis tool.")
            break
        else:
            print("Invalid choice. Please try again.")
        
        input("\nPress Enter to continue...")

def safe_analysis():
    """
    Validate data before analysis
    """
    try:
        if df.empty:
            raise ValueError("No data found in the DataFrame")
            
        if df.isnull().any().any():
            print("Warning: Data contains missing values")
            df.fillna(df.mean(), inplace=True)
            
        for column in df.columns:
            if column != 'Year' and (df[column] < 0).any():
                raise ValueError(f"Invalid negative values found in {column}")
                
        return True
        
    except Exception as e:
        print(f"Error in data validation: {str(e)}")
        return False

def main():
    """
    Main function to run the program
    """
    print("Starting Amazon Carbon Cycle Analysis...")
    
    if not os.path.exists('output'):
        os.makedirs('output')

    if not safe_analysis():
        print("Error in data validation. Please check your data.")
        return
    
    try:
        create_interactive_analysis()
        
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        print("Please check your data and try again.")

if __name__ == "__main__":
    main()