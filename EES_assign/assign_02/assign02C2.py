import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
from sklearn.linear_model import LinearRegression

class AmazonCarbonCycleModel:
    def __init__(self, historical_data):
        """Initialize with annual historical data and convert to monthly"""
        self.annual_data = historical_data
        self.monthly_data = self._convert_to_monthly()
        
    def _convert_to_monthly(self):
        """Convert annual data to monthly with seasonal adjustments"""
        # Create monthly date range
        monthly_dates = pd.date_range(
            start=f'{self.annual_data.index[0]}-01-01',
            end=f'{self.annual_data.index[-1]}-12-31',
            freq='M'
        )
        
        # Initialize monthly dataframe
        monthly_df = pd.DataFrame(index=monthly_dates)
        
        # Define seasonal adjustment factors (higher in dry season Jul-Sep)
        seasonal_factors = {
            'Deforestation Rate': [0.5, 0.5, 0.6, 0.7, 0.8, 0.9, 
                                 1.8, 1.9, 1.7, 0.9, 0.7, 0.5],
            'Fossil Fuel Emissions': [1.0, 1.0, 1.0, 0.9, 0.9, 0.9,
                                    1.1, 1.1, 1.1, 1.0, 1.0, 1.0],
            'Net Carbon Uptake': [1.2, 1.3, 1.2, 1.1, 0.9, 0.8,
                                0.7, 0.6, 0.7, 0.9, 1.1, 1.2]
        }
        
        # Interpolate annual values to monthly
        for column in self.annual_data.columns:
            # Create interpolation function
            f = interp1d(self.annual_data.index, 
                        self.annual_data[column],
                        kind='cubic', 
                        fill_value='extrapolate')
            
            # Generate monthly values
            monthly_values = f(monthly_dates.year + monthly_dates.month/12)
            
            # Apply seasonal adjustments if available
            if column in seasonal_factors:
                factors = np.tile(seasonal_factors[column], 
                                len(monthly_dates)//12 + 1)[:len(monthly_dates)]
                monthly_values *= factors
            
            monthly_df[column] = monthly_values
            
        return monthly_df
    
    def calculate_supply_days(self):
        """Calculate days where carbon uptake exceeds emissions"""
        # Convert monthly data to daily (assuming uniform distribution within month)
        days_per_month = pd.Series(monthly_df.index.days_in_month, 
                                 index=monthly_df.index)
        
        supply_days = {}
        for year in range(self.annual_data.index[0], 
                         self.annual_data.index[-1] + 1):
            year_data = self.monthly_data[
                self.monthly_data.index.year == year
            ]
            
            # Calculate days where uptake exceeds emissions
            daily_balance = year_data['Net Carbon Uptake'] > \
                          year_data['Total Carbon Emissions']
            supply_days[year] = (daily_balance * \
                               days_per_month[year_data.index]).sum()
            
        return supply_days
    
    def plot_monthly_variation(self):
        """Create plot showing monthly variations"""
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))
        
        # Plot monthly deforestation and emissions
        ax1.plot(self.monthly_data.index, 
                self.monthly_data['Deforestation Rate'],
                'r-', label='Deforestation Rate')
        ax1.plot(self.monthly_data.index,
                self.monthly_data['Fossil Fuel Emissions'],
                'b-', label='Fossil Fuel Emissions')
        ax1.set_title('Monthly Deforestation and Emission Patterns')
        ax1.legend()
        ax1.grid(True)
        
        # Plot monthly carbon balance
        ax2.plot(self.monthly_data.index,
                self.monthly_data['Total Carbon Emissions'],
                'r-', label='Total Emissions')
        ax2.plot(self.monthly_data.index,
                self.monthly_data['Net Carbon Uptake'],
                'g-', label='Net Carbon Uptake')
        ax2.set_title('Monthly Carbon Balance')
        ax2.legend()
        ax2.grid(True)
        
        plt.tight_layout()
        return fig

# Example usage
if __name__ == "__main__":
    # Create historical dataset from the provided table
    historical_data = pd.DataFrame({
        'Deforestation Rate': [2.5, 3.0, 3.8, 4.2, 4.8],
        'Carbon Released from Deforestation': [0.4, 0.5, 0.6, 0.7, 0.8],
        'Fossil Fuel Emissions': [0.6, 0.8, 1.0, 1.3, 1.5],
        'Total Carbon Emissions': [1.0, 1.3, 1.6, 2.0, 2.3],
        'Net Carbon Uptake': [1.2, 1.1, 0.9, 0.8, 0.6]
    }, index=[2000, 2005, 2010, 2015, 2020])
    
    # Initialize model
    model = AmazonCarbonCycleModel(historical_data)
    
    # Generate monthly variation plot
    monthly_fig = model.plot_monthly_variation()
    
    # Calculate supply days
    supply_days = model.calculate_supply_days()
    
    # Print supply days results
    print("\nDays where carbon uptake exceeded emissions:")
    for year, days in supply_days.items():
        print(f"{year}: {int(days)} days")
    
    plt.show()