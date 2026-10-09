#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

using namespace std;

// Engineering Constants
const double PI = 3.14159265358979323846;
const double WATER_DENSITY = 1000.0;   // kg/m^3
const double GRAVITY = 9.81;           // m/s^2
const double STEEL_COST_PER_KG = 2.50; // USD per kg

// Function Prototypes
double getValidatedInput(const string& prompt);
int getMaterialChoice();
void calculateTankMetrics(double radius, double height, double thickness, double yieldStrength,
                          double& volume, double& waterMass, double& maxPressure, 
                          double& hoopStress, double& minThickness, double& totalCost);
void printDesignReport(double radius, double height, double thickness, double volume, 
                       double waterMass, double maxPressure, double hoopStress, 
                       double minThickness, double totalCost, int materialChoice);

int main() {
    cout << "==================================================\n";
    cout << "   WATER TANK DESIGN & ANALYSIS CALCULATOR (P8)   \n";
    cout << "==================================================\n\n";

    // 1. Collect inputs with validation
    double radius = getValidatedInput("Enter tank radius (meters): ");
    double height = getValidatedInput("Enter tank height (meters): ");
    double thickness = getValidatedInput("Enter wall thickness (millimeters): ") / 1000.0; // convert mm to meters

    int materialChoice = getMaterialChoice();
    
    // Set material yield strength (Pascal) based on selection
    double yieldStrength;
    if (materialChoice == 1) {
        yieldStrength = 250e6; // Structural Steel (250 MPa)
    } else {
        yieldStrength = 355e6; // High-Strength Steel (355 MPa)
    }

    // 2. Perform Calculations
    double volume, waterMass, maxPressure, hoopStress, minThickness, totalCost;
    calculateTankMetrics(radius, height, thickness, yieldStrength, 
                         volume, waterMass, maxPressure, hoopStress, minThickness, totalCost);

    // 3. Display Results
    printDesignReport(radius, height, thickness, volume, waterMass, 
                      maxPressure, hoopStress, minThickness, totalCost, materialChoice);

    return 0;
}

// Function to validate numerical inputs (rejects negative or zero values)
double getValidatedInput(const string& prompt) {
    double value;
    cout << prompt;
    while (!(cin >> value) || value <= 0) {
        cout << "[ERROR] Invalid input. Please enter a positive number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return value;
}

// Function to get valid material choice
int getMaterialChoice() {
    int choice;
    cout << "\nSelect Tank Material:\n";
    cout << "1. Structural Steel (Yield Strength: 250 MPa)\n";
    cout << "2. High-Strength Steel (Yield Strength: 355 MPa)\n";
    cout << "Enter choice (1 or 2): ";
    
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cout << "[ERROR] Invalid choice. Please enter 1 or 2: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return choice;
}

// Core Engineering Calculations
void calculateTankMetrics(double radius, double height, double thickness, double yieldStrength,
                          double& volume, double& waterMass, double& maxPressure, 
                          double& hoopStress, double& minThickness, double& totalCost) {
    // Volume: V = pi * r^2 * h
    volume = PI * pow(radius, 2) * height;

    // Mass of water: m = density * V
    waterMass = volume * WATER_DENSITY;

    // Hydrostatic pressure at bottom: P = density * g * h
    maxPressure = WATER_DENSITY * GRAVITY * height; // in Pascals

    // Hoop Stress (thin-walled pressure vessel assumption): sigma = (P * r) / t
    hoopStress = (maxPressure * radius) / thickness; // in Pascals

    // Minimum wall thickness needed with a Safety Factor of 2.0: t_min = (P * r * SF) / yieldStrength
    double safetyFactor = 2.0;
    minThickness = (maxPressure * radius * safetyFactor) / yieldStrength; // in meters

    // Approximate steel mass (cylindrical shell + base disk) using steel density = 7850 kg/m^3
    double steelDensity = 7850.0; 
    double surfaceArea = (2 * PI * radius * height) + (PI * pow(radius, 2));
    double steelVolume = surfaceArea * thickness;
    double steelMass = steelVolume * steelDensity;

    // Material Cost estimation
    totalCost = steelMass * STEEL_COST_PER_KG;
}

// Formatted Engineering Output Report
void printDesignReport(double radius, double height, double thickness, double volume, 
                       double waterMass, double maxPressure, double hoopStress, 
                       double minThickness, double totalCost, int materialChoice) {
    cout << "\n==================================================\n";
    cout << "             ENGINEERING DESIGN REPORT            \n";
    cout << "==================================================\n";
    cout << fixed << setprecision(2);

    cout << "\n--- Tank Specifications ---\n";
    cout << "Radius:             " << radius << " m\n";
    cout << "Height:             " << height << " m\n";
    cout << "Wall Thickness:     " << thickness * 1000.0 << " mm\n";
    cout << "Material Grade:     " << (materialChoice == 1 ? "Structural Steel (250 MPa)" : "High-Strength Steel (355 MPa)") << "\n";

    cout << "\n--- Volumetric & Mass Analysis ---\n";
    cout << "Storage Volume:     " << volume << " m^3 (" << volume * 1000.0 << " Liters)\n";
    cout << "Water Mass (Full):  " << waterMass / 1000.0 << " metric tons\n";

    cout << "\n--- Structural & Pressure Analysis ---\n";
    cout << "Max Bottom Pressure:" << maxPressure / 1000.0 << " kPa\n";
    cout << "Wall Hoop Stress:   " << hoopStress / 1e6 << " MPa\n";
    cout << "Required Min Thick: " << minThickness * 1000.0 << " mm (Safety Factor = 2.0)\n";

    cout << "\n--- Cost Estimation ---\n";
    cout << "Estimated Steel Cost: $" << totalCost << " USD\n";

    cout << "\n--- Safety Verification ---\n";
    if (thickness >= minThickness) {
        cout << "STATUS: [PASS] Design thickness meets structural requirements.\n";
    } else {
        cout << "STATUS: [WARNING] Thickness is insufficient! Increase thickness to at least " 
             << minThickness * 1000.0 << " mm.\n";
    }
    cout << "==================================================\n";
}
