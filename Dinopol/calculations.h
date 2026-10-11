#ifndef CALCULATIONS_H
#define CALCULATIONS_H

// Structure to hold building data
struct Building {
    double area;           // m²
    double rValue;         // m²·°C/W
    int orientation;       // 1=North, 2=South, 3=East, 4=West
    double height;         // m
};

// Structure to hold climate data
struct Climate {
    double outdoorTemp;    // °C
    double indoorTemp;     // °C
    double annualAvgTemp;  // °C
};

// Structure to hold occupancy data
struct Occupancy {
    int people;
    double hoursPerDay;
    double equipmentLoad;  // W/m²
};

// Structure to hold monthly results
struct MonthlyResult {
    char month[10];
    double heatingLoad;    // kWh
    double coolingLoad;    // kWh
    double energyCost;     // PHP
};

// Function declarations
double calculateHeatTransfer(const Building& b, double deltaT);
double calculateHeatingLoad(const Building& b, const Climate& c, const Occupancy& o);
double calculateCoolingLoad(const Building& b, const Climate& c, const Occupancy& o);
double estimateAnnualEnergy(double monthlyHeating[], double monthlyCooling[]);
double calculateCost(double energyKwh, double ratePerKwh);
double calculateEfficiency(double cop, double load);

#endif
