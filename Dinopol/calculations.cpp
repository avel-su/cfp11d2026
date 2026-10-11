#include "calculations.h"
#include <cmath>

// Calculate heat transfer through building envelope
double calculateHeatTransfer(const Building& b, double deltaT) {
    double U = 1.0 / b.rValue;  // Heat transfer coefficient (W/m²·°C)
    double wallArea = b.area * 2.5; // Approximate wall area
    double roofArea = b.area;
    
    double heatLoss = U * (wallArea + roofArea) * deltaT;
    return heatLoss; // in Watts
}

// Calculate monthly heating load in kWh
double calculateHeatingLoad(const Building& b, const Climate& c, const Occupancy& o) {
    double deltaT = c.indoorTemp - c.outdoorTemp;
    if (deltaT <= 0) return 0.0; // No heating needed
    
    double heatTransferW = calculateHeatTransfer(b, deltaT);
    double ventilationLoad = 0.3 * b.area * 1.2 * 1005 * deltaT / 3600.0; // Simplified
    
    double totalLoadW = heatTransferW + ventilationLoad;
    double hoursPerMonth = o.hoursPerDay * 30.0;
    
    return (totalLoadW / 1000.0) * hoursPerMonth; // Convert W to kW → kWh
}

// Calculate monthly cooling load in kWh
double calculateCoolingLoad(const Building& b, const Climate& c, const Occupancy& o) {
    double deltaT = c.outdoorTemp - c.indoorTemp;
    if (deltaT <= 0) return 0.0; // No cooling needed
    
    double heatGainW = calculateHeatTransfer(b, deltaT);
    
    // Solar gain based on orientation
    double solarFactor = 0.0;
    switch(b.orientation) {
        case 1: solarFactor = 30.0; break;  // North
        case 2: solarFactor = 80.0; break;  // South
        case 3: solarFactor = 110.0; break; // East
        case 4: solarFactor = 120.0; break; // West
    }
    double solarGainW = solarFactor * b.area * 0.5;
    
    // Internal gains from people & equipment
    double peopleGainW = o.people * 100.0; // 100W per person
    double equipGainW = o.equipmentLoad * b.area;
    
    double totalLoadW = heatGainW + solarGainW + peopleGainW + equipGainW;
    double hoursPerMonth = o.hoursPerDay * 30.0;
    
    return (totalLoadW / 1000.0) * hoursPerMonth; // kWh
}

// Estimate total annual energy consumption
double estimateAnnualEnergy(double monthlyHeating[], double monthlyCooling[]) {
    double total = 0.0;
    for (int i = 0; i < 12; i++) {
        total += monthlyHeating[i] + monthlyCooling[i];
    }
    return total;
}

// Calculate energy cost in PHP
double calculateCost(double energyKwh, double ratePerKwh) {
    return energyKwh * ratePerKwh;
}

// Calculate system efficiency
double calculateEfficiency(double cop, double loadFactor) {
    return cop * loadFactor;
}
