#include "calculations.h"

const Motor motors[] = {
    {0.37, 0.75},
    {0.55, 0.78},
    {0.75, 0.80},
    {1.10, 0.82},
    {1.50, 0.84},
    {2.20, 0.86},
    {3.00, 0.87},
    {4.00, 0.88},
    {5.50, 0.89},
    {7.50, 0.90},
    {11.0, 0.91}
};

const int MOTOR_COUNT = static_cast<int>(sizeof(motors) / sizeof(motors[0]));

double calculatePower(double torqueNm, double rpm) {
    // P(kW) = torque(N.m) * speed(RPM) / 9550
    return torqueNm * rpm / 9550.0;
}

double calculateDesignPower(double outputPowerKw, double serviceFactor) {
    return outputPowerKw * serviceFactor;
}

int selectMotor(double designPowerKw) {
    for (int i = 0; i < MOTOR_COUNT; ++i) {
        if (motors[i].rating >= designPowerKw) {
            return i;
        }
    }
    return -1;
}

double calculateEfficiency(double loadFactor, double peakEfficiency) {
    // Simple illustrative model: eta = eta_peak * (0.40 + 0.60 * load).
    // This is not a real motor curve; use manufacturer data in practice.
    if (loadFactor < 0.0) loadFactor = 0.0;
    if (loadFactor > 1.0) loadFactor = 1.0;

    double efficiency = peakEfficiency * (0.40 + 0.60 * loadFactor);

    // Keep the estimate within a sensible mathematical range.
    if (efficiency < 0.01) efficiency = 0.01;
    if (efficiency > 0.99) efficiency = 0.99;
    return efficiency;
}

double calculateAnnualCost(double outputPowerKw,
                           double efficiency,
                           double hoursPerDay,
                           double daysPerYear,
                           double ratePerKWh) {
    if (efficiency <= 0.0) return 0.0;

    const double electricalInputKw = outputPowerKw / efficiency;
    const double annualEnergyKWh = electricalInputKw * hoursPerDay * daysPerYear;
    return annualEnergyKWh * ratePerKWh;
}
