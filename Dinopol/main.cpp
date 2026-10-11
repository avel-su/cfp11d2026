#include <iostream>
#include <iomanip>
#include "calculations.h"

using namespace std;

// Function prototypes
void displayMenu();
bool validateInput(double value, double minVal, double maxVal);
void generateReport(const Building& b, const Climate& c, const Occupancy& o,
                    double monthlyHeating[], double monthlyCooling[], double monthlyCost[],
                    const MonthlyResult results[]);
void compareSystems(double load);

int main() {
    cout << "======================================================" << endl;
    cout << "       HVAC SYSTEM ENERGY ESTIMATOR v1.0" << endl;
    cout << "=======================================================\n" << endl;

    Building b;
    Climate c;
    Occupancy o;
    double electricityRate = 0.0;
    
    // --------------------------
    // INPUT: Building Properties
    // --------------------------
    cout << "--- BUILDING PROPERTIES ---" << endl;
    cout << "Floor Area (m²): ";
    cin >> b.area;
    while (!validateInput(b.area, 10.0, 10000.0)) {
        cout << "  Invalid! Enter value between 10 and 10000 m²: ";
        cin >> b.area;
    }
    
    cout << "Insulation R-Value (m²·°C/W): ";
    cin >> b.rValue;
    while (!validateInput(b.rValue, 0.1, 10.0)) {
        cout << "  Invalid! Enter value between 0.1 and 10.0: ";
        cin >> b.rValue;
    }
    
    cout << "Orientation (1=North, 2=South, 3=East, 4=West): ";
    cin >> b.orientation;
    while (b.orientation < 1 || b.orientation > 4) {
        cout << "  Invalid! Enter 1-4 only: ";
        cin >> b.orientation;
    }
    
    cout << "Building Height (m): ";
    cin >> b.height;
    while (!validateInput(b.height, 2.0, 100.0)) {
        cout << "  Invalid! Enter value between 2 and 100 m: ";
        cin >> b.height;
    }
    cout << endl;

    // --------------------------
    // INPUT: Climate Data
    // --------------------------
    cout << "--- CLIMATE DATA ---" << endl;
    cout << "Outdoor Design Temperature (°C): ";
    cin >> c.outdoorTemp;
    while (!validateInput(c.outdoorTemp, -20.0, 50.0)) {
        cout << "  Invalid! Enter between -20 and 50°C: ";
        cin >> c.outdoorTemp;
    }
    
    cout << "Indoor Design Temperature (°C): ";
    cin >> c.indoorTemp;
    while (!validateInput(c.indoorTemp, 15.0, 30.0)) {
        cout << "  Invalid! Enter between 15 and 30°C: ";
        cin >> c.indoorTemp;
    }
    
    cout << "Annual Average Outdoor Temp (°C): ";
    cin >> c.annualAvgTemp;
    while (!validateInput(c.annualAvgTemp, 0.0, 40.0)) {
        cout << "  Invalid! Enter between 0 and 40°C: ";
        cin >> c.annualAvgTemp;
    }
    cout << endl;

    // --------------------------
    // INPUT: Occupancy & Usage
    // --------------------------
    cout << "--- OCCUPANCY & USAGE ---" << endl;
    cout << "Number of Occupants: ";
    cin >> o.people;
    while (!validateInput(o.people, 1.0, 500.0)) {
        cout << "  Invalid! Enter between 1 and 500: ";
        cin >> o.people;
    }
    
    cout << "Operating Hours per Day: ";
    cin >> o.hoursPerDay;
    while (!validateInput(o.hoursPerDay, 1.0, 24.0)) {
        cout << "  Invalid! Enter between 1 and 24 hours: ";
        cin >> o.hoursPerDay;
    }
    
    cout << "Equipment Load (W/m²): ";
    cin >> o.equipmentLoad;
    while (!validateInput(o.equipmentLoad, 0.0, 100.0)) {
        cout << "  Invalid! Enter between 0 and 100 W/m²: ";
        cin >> o.equipmentLoad;
    }
    cout << endl;

    // --------------------------
    // INPUT: Utility Cost
    // --------------------------
    cout << "--- UTILITY COST ---" << endl;
    cout << "Electricity Rate (PHP/kWh): ";
    cin >> electricityRate;
    while (!validateInput(electricityRate, 1.0, 30.0)) {
        cout << "  Invalid! Enter between 1.0 and 30.0 PHP/kWh: ";
        cin >> electricityRate;
    }
    cout << "\nCalculating...\n" << endl;

    // --------------------------
    // CALCULATE MONTHLY DATA
    // --------------------------
    const char* monthNames[12] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    
    MonthlyResult results[12];
    double monthlyHeating[12] = {0};
    double monthlyCooling[12] = {0};
    double monthlyCost[12] = {0};
    
    // Monthly temperature variations (simplified profile)
    double monthlyOutdoorTemp[12] = {
        24.0, 24.5, 25.5, 27.0, 28.5, 29.0,
        28.5, 28.0, 27.5, 26.5, 25.5, 24.5
    };
    
    double totalHeatingKwh = 0.0;
    double totalCoolingKwh = 0.0;

    for (int i = 0; i < 12; i++) {
        // Adjust outdoor temp for each month
        Climate monthlyClimate = c;
        monthlyClimate.outdoorTemp = monthlyOutdoorTemp[i];
        
        // Calculate loads
        monthlyHeating[i] = calculateHeatingLoad(b, monthlyClimate, o);
        monthlyCooling[i] = calculateCoolingLoad(b, monthlyClimate, o);
        
        // Apply system COP efficiency (typical AC: COP ≈ 3.0)
        double coolingCOP = 3.0;
        double heatingCOP = 2.5;
        double coolingEnergy = monthlyCooling[i] / coolingCOP;
        double heatingEnergy = monthlyHeating[i] / heatingCOP;
        
        totalHeatingKwh += heatingEnergy;
        totalCoolingKwh += coolingEnergy;
        
        // Calculate cost
        monthlyCost[i] = calculateCost(heatingEnergy + coolingEnergy, electricityRate);
        
        // Store results
        snprintf(results[i].month, sizeof(results[i].month), "%s", monthNames[i]);
        results[i].heatingLoad = heatingEnergy;
        results[i].coolingLoad = coolingEnergy;
        results[i].energyCost = monthlyCost[i];
    }

    // --------------------------
    // GENERATE REPORT
    // --------------------------
    cout << "=======================================================" << endl;
    cout << "           HVAC ENERGY CONSUMPTION REPORT" << endl;
    cout << "=======================================================\n" << endl;
    
    cout << "--- MONTHLY BREAKDOWN ---" << endl;
    cout << left << setw(12) << "Month" 
         << right << setw(12) << "Heating (kWh)" 
         << setw(12) << "Cooling (kWh)" 
         << setw(14) << "Cost (PHP)" << endl;
    cout << string(52, '-') << endl;
    
    cout << fixed << setprecision(2);
    for (int i = 0; i < 12; i++) {
        cout << left << setw(12) << results[i].month
             << right << setw(12) << results[i].heatingLoad
             << setw(12) << results[i].coolingLoad
             << setw(14) << results[i].energyCost << endl;
    }
    
    cout << string(52, '-') << endl;
    cout << left << setw(12) << "ANNUAL TOTAL"
         << right << setw(12) << totalHeatingKwh
         << setw(12) << totalCoolingKwh
         << setw(14) << calculateCost(totalHeatingKwh + totalCoolingKwh, electricityRate) 
         << "\n" << endl;

    // --------------------------
    // SYSTEM COMPARISON
    // --------------------------
    cout << "--- SYSTEM EFFICIENCY COMPARISON ---" << endl;
    double avgLoad = (totalHeatingKwh + totalCoolingKwh) / 8760.0; // kW
    
    cout << setw(25) << "Standard AC (COP=3.0):" 
         << setw(10) << calculateCost((totalHeatingKwh/2.5 + totalCoolingKwh/3.0), electricityRate) 
         << " PHP/year" << endl;
    cout << setw(25) << "High-Efficiency AC (COP=4.5):" 
         << setw(10) << calculateCost((totalHeatingKwh/3.5 + totalCoolingKwh/4.5), electricityRate) 
         << " PHP/year" << endl;
    cout << setw(25) << "Estimated Annual Savings:" 
         << setw(10) << calculateCost((totalHeatingKwh/2.5 + totalCoolingKwh/3.0) - 
                                       (totalHeatingKwh/3.5 + totalCoolingKwh/4.5), electricityRate) 
         << " PHP\n" << endl;

    cout << "=======================================================" << endl;
    cout << "  REPORT COMPLETE — Thank you for using DolaHVAC!" << endl;
    cout << "=======================================================" << endl;
    
    return 0;
}

// Input validation function
bool validateInput(double value, double minVal, double maxVal) {
    return (value >= minVal && value <= maxVal);
}
void displayMenu() {
    cout << "\n===== HVAC ENERGY ESTIMATOR =====\n";
    cout << "1. Run New Calculation\n";
    cout << "2. Exit\n";
    cout << "===================================\n";
}

void generateReport(const Building& b, const Climate& c, const Occupancy& o,
                    double monthlyHeating[], double monthlyCooling[], double monthlyCost[],
                    const MonthlyResult results[]) {
    // Report generation handled in main()
}

void compareSystems(double load) {
    // System comparison handled in main()
}
