import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
from sklearn.linear_model import LinearRegression

class AmazonCarbonCycleModel:
    def __init__(self, historical_data):
        """
        Initialize the carbon cycle model with historical data.
        
        Parameters:
        historical_data (pd.DataFrame): DataFrame containing historical carbon flux data
        """
        self.historical_data = historical_data
        self.years = historical_data.index.values
        
        # Create interpolation functions for each measure
        self._create_interpolation_functions()
        
        # Initialize prediction models
        self._initialize_prediction_models()
    
    def _create_interpolation_functions(self):
        """Create interpolation functions for smooth transitions between data points"""
        self.deforestation_interp = interp1d(self.years, 
                                           self.historical_data['Deforestation Rate'],
                                           kind='cubic', 
                                           fill_value='extrapolate')
        
        self.fossil_fuel_interp = interp1d(self.years, 
                                          self.historical_data['Fossil Fuel Emissions'],
                                          kind='cubic', 
                                          fill_value='extrapolate')
        
        self.carbon_uptake_interp = interp1d(self.years, 
                                            self.historical_data['Net Carbon Uptake'],
                                            kind='cubic', 
                                            fill_value='extrapolate')
    
    def _initialize_prediction_models(self):
        """Initialize linear regression models for future predictions"""
        X = self.years.reshape(-1, 1)
        
        # Train models for each variable
        self.deforestation_model = LinearRegression().fit(
            X, self.historical_data['Deforestation Rate'])
        
        self.emissions_model = LinearRegression().fit(
            X, self.historical_data['Fossil Fuel Emissions'])
        
        self.uptake_model = LinearRegression().fit(
            X, self.historical_data['Net Carbon Uptake'])
    
    def predict_future_scenarios(self, end_year, 
                               deforestation_reduction=0.0, 
                               emission_reduction=0.0):
        """
        Predict future carbon cycle scenarios with optional reduction measures.
        
        Parameters:
        end_year (int): Year until which to make predictions
        deforestation_reduction (float): Percentage reduction in deforestation (0-1)
        emission_reduction (float): Percentage reduction in fossil fuel emissions (0-1)
        
        Returns:
        pd.DataFrame: Predicted values for all variables
        """
        future_years = np.arange(self.years[-1] + 1, end_year + 1)
        X_future = future_years.reshape(-1, 1)
        
        # Generate baseline predictions
        deforestation_pred = self.deforestation_model.predict(X_future)
        emissions_pred = self.emissions_model.predict(X_future)
        uptake_pred = self.uptake_model.predict(X_future)
        
        # Apply reduction factors
        deforestation_pred *= (1 - deforestation_reduction)
        emissions_pred *= (1 - emission_reduction)
        
        # Calculate total emissions
        total_emissions_pred = (deforestation_pred * 0.167) + emissions_pred
        
        # Create prediction DataFrame
        predictions = pd.DataFrame({
            'Deforestation Rate': deforestation_pred,
            'Carbon Released from Deforestation': deforestation_pred * 0.167,
            'Fossil Fuel Emissions': emissions_pred,
            'Total Carbon Emissions': total_emissions_pred,
            'Net Carbon Uptake': uptake_pred
        }, index=future_years)
        
        return predictions
    
    def visualize_historical_trends(self):
        """Create visualization of historical carbon flux data"""
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))
        
        # Plot deforestation and emissions trends
        ax1.plot(self.years, self.historical_data['Deforestation Rate'], 
                'r-', label='Deforestation Rate (Mha/yr)')
        ax1.plot(self.years, self.historical_data['Fossil Fuel Emissions'], 
                'b-', label='Fossil Fuel Emissions (Gt CO₂/yr)')
        ax1.set_title('Historical Deforestation and Emission Trends')
        ax1.legend()
        ax1.grid(True)
        
        # Plot carbon balance
        ax2.plot(self.years, self.historical_data['Total Carbon Emissions'], 
                'r-', label='Total Emissions')
        ax2.plot(self.years, self.historical_data['Net Carbon Uptake'], 
                'g-', label='Net Carbon Uptake')
        ax2.set_title('Carbon Balance Over Time')
        ax2.legend()
        ax2.grid(True)
        
        plt.tight_layout()
        return fig
    
    def visualize_future_scenarios(self, predictions_baseline, 
                                 predictions_reduction, end_year):
        """
        Create visualization comparing baseline and reduction scenarios
        
        Parameters:
        predictions_baseline (pd.DataFrame): Predictions without reduction measures
        predictions_reduction (pd.DataFrame): Predictions with reduction measures
        end_year (int): End year of predictions
        """
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))
        
        # Historical data
        historical_years = np.arange(2000, 2025, 5)
        
        # Plot emissions scenarios
        ax1.plot(historical_years, 
                self.historical_data['Total Carbon Emissions'], 
                'k-', label='Historical')
        ax1.plot(predictions_baseline.index, 
                predictions_baseline['Total Carbon Emissions'], 
                'r--', label='Baseline Scenario')
        ax1.plot(predictions_reduction.index, 
                predictions_reduction['Total Carbon Emissions'], 
                'g--', label='Reduction Scenario')
        ax1.set_title('Total Carbon Emissions Scenarios')
        ax1.legend()
        ax1.grid(True)
        
        # Plot net carbon uptake scenarios
        ax2.plot(historical_years, 
                self.historical_data['Net Carbon Uptake'], 
                'k-', label='Historical')
        ax2.plot(predictions_baseline.index, 
                predictions_baseline['Net Carbon Uptake'], 
                'r--', label='Baseline Scenario')
        ax2.plot(predictions_reduction.index, 
                predictions_reduction['Net Carbon Uptake'], 
                'g--', label='Reduction Scenario')
        ax2.set_title('Net Carbon Uptake Scenarios')
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
    
    # Generate predictions for different scenarios
    end_year = 2070
    baseline_predictions = model.predict_future_scenarios(end_year)
    reduction_predictions = model.predict_future_scenarios(
        end_year, 
        deforestation_reduction=0.5,
        emission_reduction=0.3
    )
    
    # Visualize results
    historical_fig = model.visualize_historical_trends()
    future_fig = model.visualize_future_scenarios(
        baseline_predictions, 
        reduction_predictions, 
        end_year
    )
    
    plt.show()