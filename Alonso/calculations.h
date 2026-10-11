#ifndef CALCULATIONS_H
#define CALCULATIONS_H

// Educational motor-selection model for P9.
// Ratings and peak efficiencies are sample values, not manufacturer data.

struct Motor {
    double rating;          // Rated mechanical output power, kW
    double peakEfficiency;  // Estimated peak efficiency as a fraction
};

extern const Motor motors[];
extern const int MOTOR_COUNT;

double calculatePower(double torqueNm, double rpm);
double calculateDesignPower(double outputPowerKw, double serviceFactor);
int selectMotor(double designPowerKw);

// Simplified educational estimate; not a manufacturer efficiency curve.
double calculateEfficiency(double loadFactor, double peakEfficiency);

double calculateAnnualCost(double outputPowerKw,
                           double efficiency,
                           double hoursPerDay,
                           double daysPerYear,
                           double ratePerKWh);

#endif
