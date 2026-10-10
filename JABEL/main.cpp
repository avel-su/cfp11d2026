#include "calculations.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "=================================================\n";
    std::cout << "     HVAC System Energy & Cost Estimator (C++)   \n";
    std::cout << "=================================================\n";

    std::vector<double> defaultTemps = {32.0, 35.0, 42.0, 52.0, 63.0, 72.0, 78.0, 76.0, 68.0, 56.0, 45.0, 35.0};

    try {
        BuildingProperties building = collectBuildingProperties();
        SystemParameters systems = collectSystemParameters();

        std::cout << "\nRunning monthly energy & utility calculations...\n";
        std::vector<MonthlyResult> monthlyResults = calculateMonthlyEnergy(building, systems, defaultTemps);

        generateEnergyReport(monthlyResults);
        runSystemComparison(building, systems, defaultTemps);

    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\nProgram execution completed successfully.\n";
    return 0;
}