#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// Function declarations
double calculateInputPower(double ratedHP, double loadFactor, double efficiencyPercent) {
    // 1 HP = 0.746 kW
    double outputPowerKW = ratedHP * 0.746 * loadFactor;
    double efficiencyDecimal = efficiencyPercent / 100.0;
    return outputPowerKW / efficiencyDecimal;
}

double calculateAnnualCost(double inputPowerKW, double hoursPerYear, double costPerKWh) {
    return inputPowerKW * hoursPerYear * costPerKWh;
}

int main() {
    cout << "==================================================" << endl;
    cout << "   P9: Motor Selection & Efficiency Calculator   " << endl;
    cout << "==================================================" << endl;

    double ratedHP, loadFactor, efficiencyPercent, hoursPerYear, costPerKWh;

    // User Inputs
    cout << "Enter Motor Rated Horsepower (HP): ";
    cin >> ratedHP;

    cout << "Enter Load Factor (e.g., 0.75 for 75% load): ";
    cin >> loadFactor;

    cout << "Enter Efficiency percentage (e.g., 90 for 90%): ";
    cin >> efficiencyPercent;

    cout << "Enter Operating Hours per Year: ";
    cin >> hoursPerYear;

    cout << "Enter Energy Cost per kWh ($): ";
    cin >> costPerKWh;

    // Calculations
    double inputPowerKW = calculateInputPower(ratedHP, loadFactor, efficiencyPercent);
    double annualCost = calculateAnnualCost(inputPowerKW, hoursPerYear, costPerKWh);

    // Display Results
    cout << "\n------------------ RESULTS ------------------" << endl;
    cout << fixed << setprecision(2);
    cout << "Input Power Required : " << inputPowerKW << " kW" << endl;
    cout << "Annual Operating Cost: $" << annualCost << endl;
    cout << "---------------------------------------------" << endl;

    return 0;
}
