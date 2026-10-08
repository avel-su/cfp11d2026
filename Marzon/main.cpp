#include <iostream>
#include <cmath>
#include <iomanip>

// Function Declarations
void displayHeader();
void getValidInput(const std::string& prompt, double& value);
double calculateReynoldsNumber(double density, double velocity, double diameter, double viscosity);
double calculateFrictionFactor(double reynolds, double roughness, double diameter);
double calculatePressureDrop(double frictionFactor, double length, double diameter, double density, double velocity);
void displayReport(double diameter, double length, double roughness, double density, 
                   double viscosity, double velocity, double reynolds, 
                   double frictionFactor, double pressureDropPa);

int main() {
    displayHeader();

    double diameter, length, roughness, density, viscosity, velocity;

    std::cout << "--- INPUT FLUID & PIPE PARAMETERS ---\n";
    getValidInput("Enter Pipe Inner Diameter (m): ", diameter);
    getValidInput("Enter Pipe Length (m): ", length);
    getValidInput("Enter Pipe Surface Roughness (m): ", roughness);
    getValidInput("Enter Fluid Density (kg/m^3): ", density);
    getValidInput("Enter Fluid Dynamic Viscosity (Pa*s): ", viscosity);
    getValidInput("Enter Average Flow Velocity (m/s): ", velocity);

    // Perform Fluid Dynamics Calculations
    double reynolds = calculateReynoldsNumber(density, velocity, diameter, viscosity);
    double frictionFactor = calculateFrictionFactor(reynolds, roughness, diameter);
    double pressureDropPa = calculatePressureDrop(frictionFactor, length, diameter, density, velocity);

    // Display Output Report
    displayReport(diameter, length, roughness, density, viscosity, velocity, reynolds, frictionFactor, pressureDropPa);

    return 0;
}

// Function Implementations

void displayHeader() {
    std::cout << "====================================================\n";
    std::cout << "     PIPE FLOW & PRESSURE DROP CALCULATOR (P3)      \n";
    std::cout << "     Student: Marzon | nickcmarzon@su.edu.ph       \n";
    std::cout << "====================================================\n\n";
}

// Input validation to ensure valid positive physical measurements
void getValidInput(const std::string& prompt, double& value) {
    do {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value <= 0.0) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input. Please enter a positive numerical value.\n";
        } else {
            break;
        }
    } while (true);
}

// Reynolds Number Calculation: Re = (rho * v * D) / mu
double calculateReynoldsNumber(double density, double velocity, double diameter, double viscosity) {
    return (density * velocity * diameter) / viscosity;
}

// Friction Factor: 64/Re for Laminar, Haaland equation for Turbulent flow
double calculateFrictionFactor(double reynolds, double roughness, double diameter) {
    if (reynolds < 2300.0) {
        // Laminar Flow
        return 64.0 / reynolds;
    } else {
        // Turbulent / Transitional Flow (Haaland Equation)
        double relativeRoughness = roughness / diameter;
        double term = std::pow(relativeRoughness / 3.7, 1.11) + (6.9 / reynolds);
        double invSqrtF = -1.8 * std::log10(term);
        return 1.0 / (invSqrtF * invSqrtF);
    }
}

// Darcy-Weisbach Equation: deltaP = f * (L / D) * (rho * v^2 / 2)
double calculatePressureDrop(double frictionFactor, double length, double diameter, double density, double velocity) {
    return frictionFactor * (length / diameter) * (density * velocity * velocity / 2.0);
}

// Output Results Summary
void displayReport(double diameter, double length, double roughness, double density, 
                   double viscosity, double velocity, double reynolds, 
                   double frictionFactor, double pressureDropPa) {
    std::cout << "\n====================================================\n";
    std::cout << "                 CALCULATION RESULTS                \n";
    std::cout << "====================================================\n";
    std::cout << std::fixed << std::setprecision(4);

    std::cout << "Inputs Summary:\n";
    std::cout << "  - Diameter:         " << diameter << " m\n";
    std::cout << "  - Length:           " << length << " m\n";
    std::cout << "  - Roughness:        " << roughness << " m\n";
    std::cout << "  - Density:          " << density << " kg/m^3\n";
    std::cout << "  - Viscosity:        " << viscosity << " Pa*s\n";
    std::cout << "  - Velocity:         " << velocity << " m/s\n\n";

    std::cout << "Hydrodynamic Analysis:\n";
    std::cout << "  - Reynolds Number:  " << reynolds << "\n";
    std::cout << "  - Flow Regime:      " << (reynolds < 2300.0 ? "Laminar" : "Turbulent") << "\n";
    std::cout << "  - Friction Factor:  " << frictionFactor << "\n\n";

    std::cout << "Pressure Loss Results:\n";
    std::cout << "  - Pressure Drop:    " << pressureDropPa << " Pa\n";
    std::cout << "  - Pressure Drop:    " << (pressureDropPa / 1000.0) << " kPa\n";
    std::cout << "====================================================\n";
}
