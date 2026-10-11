#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <string>
#include "calculations.h"

using namespace std;

double getPositiveInput(const string& prompt, double minimum, double maximum) {
    double value;

    while (true) {
        cout << prompt;

        if (cin >> value && isfinite(value) &&
            value >= minimum && value <= maximum) {
            return value;
        }

        cout << "Invalid input. Enter a number from "
             << minimum << " to " << maximum << ".\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void displayMotorReport(int index,
                        double outputPowerKw,
                        double hoursPerDay,
                        double daysPerYear,
                        double ratePerKWh) {
    const double loadFactor = outputPowerKw / motors[index].rating;
    const double efficiency =
        calculateEfficiency(loadFactor, motors[index].peakEfficiency);
    const double electricalInputKw = outputPowerKw / efficiency;
    const double annualCost =
        calculateAnnualCost(outputPowerKw, efficiency,
                            hoursPerDay, daysPerYear, ratePerKWh);

    cout << fixed << setprecision(2);
    cout << "\nMotor rating: " << motors[index].rating << " kW\n";
    cout << "Operating load: " << loadFactor * 100.0 << "%\n";
    cout << "Estimated efficiency: " << efficiency * 100.0 << "%\n";
    cout << "Estimated electrical input: " << electricalInputKw << " kW\n";
    cout << "Estimated annual electricity cost: PHP " << annualCost << "\n";
}

int main() {
    cout << "========================================\n";
    cout << " MOTOR SELECTION & EFFICIENCY CALCULATOR\n";
    cout << "========================================\n";
    cout << "\nEnter machine operating data:\n";

    const double torqueNm =
        getPositiveInput("Torque (N.m, 0.01 to 100000): ", 0.01, 100000.0);
    const double rpm =
        getPositiveInput("Speed (RPM, 1 to 100000): ", 1.0, 100000.0);
    const double serviceFactor =
        getPositiveInput("Service factor (1.0 to 2.0): ", 1.0, 2.0);

    cout << "\nEnter operating cost data:\n";
    const double hoursPerDay =
        getPositiveInput("Operating hours per day (0.1 to 24): ", 0.1, 24.0);
    const double daysPerYear =
        getPositiveInput("Operating days per year (1 to 365): ", 1.0, 365.0);
    const double ratePerKWh =
        getPositiveInput("Electricity rate (PHP/kWh, 0.01 to 1000): ", 0.01, 1000.0);

    const double outputPowerKw = calculatePower(torqueNm, rpm);
    const double designPowerKw = calculateDesignPower(outputPowerKw, serviceFactor);

    cout << fixed << setprecision(2);
    cout << "\n========== ENGINEERING REPORT ==========\n";
    cout << "Mechanical output power: " << outputPowerKw << " kW\n";
    cout << "Service factor: " << serviceFactor << "\n";
    cout << "Required design power: " << designPowerKw << " kW\n";

    const int selected = selectMotor(designPowerKw);

    if (selected == -1) {
        cout << "\nNo suitable motor is available in the sample catalog.\n";
        cout << "Consider a larger motor rating and verify it with manufacturer data.\n";
        cout << "\nNOTE: This is an educational estimate, not a final engineering selection.\n";
        return 0;
    }

    cout << "\n========== MOTOR COMPARISON ============\n";
    cout << "\n--- Smallest motor meeting design power ---\n";
    displayMotorReport(selected, outputPowerKw,
                       hoursPerDay, daysPerYear, ratePerKWh);

    if (selected + 1 < MOTOR_COUNT) {
        const int larger = selected + 1;

        cout << "\n--- Next larger motor ---\n";
        displayMotorReport(larger, outputPowerKw,
                           hoursPerDay, daysPerYear, ratePerKWh);

        const double efficiency1 = calculateEfficiency(
            outputPowerKw / motors[selected].rating,
            motors[selected].peakEfficiency);
        const double efficiency2 = calculateEfficiency(
            outputPowerKw / motors[larger].rating,
            motors[larger].peakEfficiency);

        const double cost1 = calculateAnnualCost(
            outputPowerKw, efficiency1, hoursPerDay, daysPerYear, ratePerKWh);
        const double cost2 = calculateAnnualCost(
            outputPowerKw, efficiency2, hoursPerDay, daysPerYear, ratePerKWh);

        cout << "\n========== COMPARISON RESULT ===========\n";
        if (cost1 < cost2) {
            cout << motors[selected].rating
                 << " kW motor has the lower estimated annual cost.\n";
        } else if (cost2 < cost1) {
            cout << motors[larger].rating
                 << " kW motor has the lower estimated annual cost.\n";
        } else {
            cout << "Both motors have equal estimated annual costs.\n";
        }

        cout << "Estimated annual cost difference: PHP "
             << abs(cost1 - cost2) << "\n";
    } else {
        cout << "\nNo larger motor is available for comparison.\n";
    }

    cout << "\nNOTE: Motor ratings and efficiency values are sample educational data.\n";
    cout << "The efficiency model is simplified; use manufacturer specifications for real selection.\n";
    cout << "========================================\n";

    return 0;
}
