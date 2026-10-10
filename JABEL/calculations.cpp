#include "calculations.h"
#include <iostream>
#include <iomanip>
#include <limits>

double getValidatedInput(const std::string& prompt, double minVal, bool allowZero) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Error: Invalid input. Please enter a numerical value.\n";
            continue;
        }
        if (allowZero && value < minVal) {
            std::cout << "Error: Value must be greater than or equal to " << minVal << ".\n";
            continue;
        }
        if (!allowZero && value <= minVal) {
            std::cout << "Error: Value must be strictly greater than " << minVal << ".\n";
            continue;
        }
        return value;
    }
}

BuildingProperties collectBuildingProperties() {
    std::cout << "\n--- Step 1: Building Thermal Envelope Properties ---\n";
    double area = getValidatedInput("Enter total conditioned floor area (sq ft): ", 0.0, false);
    double rValue = getValidatedInput("Enter insulation thermal resistance (R-value, h·sq ft·°F/BTU): ", 0.0, false);
    double setpointHeat = getValidatedInput("Enter winter indoor heating setpoint (°F): ", 30.0, true);
    double setpointCool = getValidatedInput("Enter summer indoor cooling setpoint (°F): ", 50.0, true);
    return {area, rValue, setpointHeat, setpointCool};
}

SystemParameters collectSystemParameters() {
    std::cout << "\n--- Step 2: HVAC System & Utility Rates ---\n";
    double hspf = getValidatedInput("Enter Heating Seasonal Performance Factor (HSPF, e.g., 8.0): ", 0.1, false);
    double seer = getValidatedInput("Enter Seasonal Energy Efficiency Ratio (SEER, e.g., 14.0): ", 0.1, false);
    double rate = getValidatedInput("Enter electricity utility rate ($/kWh): ", 0.0, true);
    return {hspf, seer, rate};
}

std::vector<MonthlyResult> calculateMonthlyEnergy(const BuildingProperties& building, const SystemParameters& systems, const std::vector<double>& monthlyTemps) {
    std::vector<MonthlyResult> results;
    const std::vector<std::string> months = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    
    for (size_t i = 0; i < monthlyTemps.size(); ++i) {
        double temp = monthlyTemps[i];
        std::string mode = "None";
        double kwh = 0.0;
        double cost = 0.0;

        if (temp < building.heatingSetpoint) {
            mode = "Heating";
            double deltaT = building.heatingSetpoint - temp;
            double btuLoad = (building.area / building.rValue) * deltaT * 24.0 * 30.0;
            kwh = btuLoad / systems.hspf / 1000.0;
            cost = kwh * systems.electricityRate;
        } else if (temp > building.coolingSetpoint) {
            mode = "Cooling";
            double deltaT = temp - building.coolingSetpoint;
            double btuLoad = (building.area / building.rValue) * deltaT * 24.0 * 30.0;
            kwh = btuLoad / systems.seer / 1000.0;
            cost = kwh * systems.electricityRate;
        }

        results.push_back({months[i], temp, mode, kwh, cost});
    }
    return results;
}

void generateEnergyReport(const std::vector<MonthlyResult>& monthlyData) {
    std::cout << "\n" << std::string(65, '=') << "\n";
    std::cout << std::setw(42) << "HVAC SYSTEM ANNUAL ENERGY & COST REPORT\n";
    std::cout << std::string(65, '=') << "\n";
    std::cout << std::left << std::setw(8) << "Month" 
              << std::setw(15) << "Avg Temp (°F)" 
              << std::setw(10) << "Mode" 
              << std::setw(16) << "Energy (kWh)" 
              << std::setw(10) << "Cost ($)" << "\n";
    std::cout << std::string(65, '-') << "\n";

    double totalKwh = 0.0;
    double totalCost = 0.0;

    for (const auto& row : monthlyData) {
        std::cout << std::left << std::setw(8) << row.month
                  << std::setw(15) << std::fixed << std::setprecision(1) << row.avgTemp
                  << std::setw(10) << row.mode
                  << std::setw(16) << std::fixed << std::setprecision(2) << row.energyKWh
                  << "$" << std::setw(9) << std::fixed << std::setprecision(2) << row.cost << "\n";
        totalKwh += row.energyKWh;
        totalCost += row.cost;
    }

    std::cout << std::string(65, '-') << "\n";
    std::cout << std::left << std::setw(8) << "TOTAL"
              << std::setw(15) << "-"
              << std::setw(10) << "-"
              << std::setw(16) << std::fixed << std::setprecision(2) << totalKwh
              << "$" << std::setw(9) << std::fixed << std::setprecision(2) << totalCost << "\n";
    std::cout << std::string(65, '=') << "\n";
}

void runSystemComparison(const BuildingProperties& building, const SystemParameters& systems, const std::vector<double>& monthlyTemps) {
    std::cout << "\n--- Efficiency Upgrade Comparison Analysis ---\n";
    auto baselineResults = calculateMonthlyEnergy(building, systems, monthlyTemps);
    double baselineCost = 0.0;
    for (const auto& m : baselineResults) baselineCost += m.cost;

    BuildingProperties upgradedBuilding = building;
    upgradedBuilding.rValue *= 1.5;

    SystemParameters upgradedSystems = systems;
    upgradedSystems.hspf *= 1.25;
    upgradedSystems.seer *= 1.25;

    auto upgradedResults = calculateMonthlyEnergy(upgradedBuilding, upgradedSystems, monthlyTemps);
    double upgradedCost = 0.0;
    for (const auto& m : upgradedResults) upgradedCost += m.cost;

    double savings = baselineCost - upgradedCost;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Baseline Annual Cost:     $" << baselineCost << "\n";
    std::cout << "Upgraded System Cost:     $" << upgradedCost << "\n";
    std::cout << "Estimated Annual Savings: $" << savings << " (" << (savings / baselineCost) * 100.0 << "% reduction)\n";
    std::cout << std::string(45, '-') << "\n";
}