#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <string>
#include <vector>

struct BuildingProperties {
    double area;
    double rValue;
    double heatingSetpoint;
    double coolingSetpoint;
};

struct SystemParameters {
    double hspf;
    double seer;
    double electricityRate;
};

struct MonthlyResult {
    std::string month;
    double avgTemp;
    std::string mode;
    double energyKWh;
    double cost;
};

double getValidatedInput(const std::string& prompt, double minVal, bool allowZero);
BuildingProperties collectBuildingProperties();
SystemParameters collectSystemParameters();
std::vector<MonthlyResult> calculateMonthlyEnergy(const BuildingProperties& building, const SystemParameters& systems, const std::vector<double>& monthlyTemps);
void generateEnergyReport(const std::vector<MonthlyResult>& monthlyData);
void runSystemComparison(const BuildingProperties& building, const SystemParameters& systems, const std::vector<double>& monthlyTemps);

#endif