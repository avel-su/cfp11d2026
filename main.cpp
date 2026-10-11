
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <string>
using namespace std;

// Engineering constants
const double PI = 3.141592653589793;
const double GRAVITY = 9.81;

// Checks whether a value is positive
bool isPositive(double value) {
    return value > 0.0;
}

// Safely reads a number from the user
double getNumber(string prompt) {
    double value;

    while (true) {
        cout << prompt;
        cin >> value;

        if (!cin.fail()) {
            return value;
        }

        cout << "Invalid input. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Calculates the pipe cross-sectional area
double calculateArea(double diameter) {
    return PI * diameter * diameter / 4.0;
}

// Calculates average fluid velocity
double calculateVelocity(double flowRate, double area) {
    return flowRate / area;
}

// Calculates the Reynolds number
double calculateReynolds(double density, double velocity,
                          double diameter, double viscosity) {
    return density * velocity * diameter / viscosity;
}

// Calculates the Darcy friction factor using the Haaland equation
double calculateFrictionFactor(double reynolds, double diameter,
                                double roughness) {
    double term = pow(roughness / (3.7 * diameter), 1.11)
                  + 6.9 / reynolds;

    double inverseSqrtF = -1.8 * log10(term);

    return 1.0 / (inverseSqrtF * inverseSqrtF);
}

// Classifies the flow regime
string determineFlowRegime(double reynolds) {
    if (reynolds < 2300.0) {
        return "Laminar";
    } else if (reynolds <= 4000.0) {
        return "Transitional";
    } else {
        return "Turbulent";
    }
}

// Performs one pipe flow calculation
void calculatePipeFlow() {
    double flowRate, diameter, length;
    double density, viscosity, roughness;

    cout << "\n--- ENTER PIPE AND FLUID DATA ---\n";

    while (true) {
        flowRate = getNumber("Volumetric flow rate (m^3/s): ");
        if (isPositive(flowRate)) break;
        cout << "Flow rate must be greater than zero.\n";
    }

    while (true) {
        diameter = getNumber("Pipe inside diameter (m): ");
        if (isPositive(diameter)) break;
        cout << "Diameter must be greater than zero.\n";
    }

    while (true) {
        length = getNumber("Pipe length (m): ");
        if (isPositive(length)) break;
        cout << "Length must be greater than zero.\n";
    }

    while (true) {
        density = getNumber("Fluid density (kg/m^3): ");
        if (isPositive(density)) break;
        cout << "Density must be greater than zero.\n";
    }

    while (true) {
        viscosity = getNumber("Dynamic viscosity (Pa.s): ");
        if (isPositive(viscosity)) break;
        cout << "Viscosity must be greater than zero.\n";
    }

    while (true) {
        roughness = getNumber("Pipe absolute roughness (m): ");
        if (roughness >= 0.0) break;
        cout << "Roughness cannot be negative.\n";
    }

    // Calculate area, velocity, and Reynolds number
    double area = calculateArea(diameter);
    double velocity = calculateVelocity(flowRate, area);

    double reynolds = calculateReynolds(
        density, velocity, diameter, viscosity
    );

    string regime = determineFlowRegime(reynolds);
    double frictionFactor;

    if (reynolds < 2300.0) {
        // Darcy friction factor for fully developed laminar flow
        frictionFactor = 64.0 / reynolds;
    } else {
        // Haaland approximation; transitional results are uncertain
        frictionFactor = calculateFrictionFactor(
            reynolds, diameter, roughness
        );
    }

    // Darcy-Weisbach equation for straight-pipe pressure loss
    double pressureDrop = frictionFactor
        * (length / diameter)
        * (density * velocity * velocity / 2.0);

    // Convert pressure loss to equivalent fluid head
    double headLoss = pressureDrop / (density * GRAVITY);

    // Display results
    cout << fixed << setprecision(6);

    cout << "\n============================================\n";
    cout << "             CALCULATION RESULTS\n";
    cout << "============================================\n";
    cout << "Pipe area             : " << area << " m^2\n";
    cout << "Flow velocity         : " << velocity << " m/s\n";
    cout << "Reynolds number       : " << reynolds << "\n";
    cout << "Flow regime           : " << regime << "\n";
    cout << "Darcy friction factor : " << frictionFactor << "\n";
    cout << "Pressure drop         : " << pressureDrop << " Pa\n";
    cout << "Head loss             : " << headLoss << " m\n";
    cout << "============================================\n";

    if (regime == "Transitional") {
        cout << "WARNING: Transitional flow is unpredictable.\n";
        cout << "The friction factor and losses are estimates.\n";
    }

    cout << "\nAssumption: Straight, circular pipe with steady flow.\n";
    cout << "Fittings and other minor losses are not included.\n";
}

// Main menu
int main() {
    int choice;

    do {
        cout << "\n============================================\n";
        cout << "       PIPE FLOW & PRESSURE CALCULATOR\n";
        cout << "============================================\n";
        cout << "1. Calculate pipe flow and pressure drop\n";
        cout << "2. Exit\n";
        cout << "============================================\n";

        while (true) {
            double input = getNumber("Enter your choice: ");

            if (input == 1 || input == 2) {
                choice = static_cast<int>(input);
                break;
            }

            cout << "Please enter 1 or 2 only.\n";
        }

        if (choice == 1) {
            calculatePipeFlow();
        } else {
            cout << "\nThank you for using the calculator!\n";
        }

    } while (choice != 2);

    return 0;
}
