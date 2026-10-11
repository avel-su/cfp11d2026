// calculations.h
// Pipe Flow & Pressure Drop Calculator - engineering calculation module
//
// UNITS (SI throughout):
//   length, diameter, roughness : m
//   velocity                    : m/s
//   flow rate (discharge)       : m^3/s
//   density                     : kg/m^3
//   dynamic viscosity           : Pa.s
//   pressure                    : Pa
#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <string>

namespace pipeflow {

// ---- Constants & limits ---------------------------------------------------
const double PI            = 3.14159265358979323846;
const double GRAVITY       = 9.80665;   // m/s^2 (standard gravity)
const double RE_LAMINAR    = 2300.0;    // Re below this: laminar
const double RE_TURBULENT  = 4000.0;    // Re above this: turbulent (between = transitional)

// Validation limits (physically reasonable for engineering pipe flow)
const double MIN_DIAMETER  = 0.001, MAX_DIAMETER  = 5.0;      // m
const double MAX_LENGTH    = 1.0e5;                           // m
const double MIN_DENSITY   = 0.1,   MAX_DENSITY   = 25000.0;  // kg/m^3
const double MIN_VISCOSITY = 1.0e-6, MAX_VISCOSITY = 10.0;    // Pa.s
const double MAX_VELOCITY  = 100.0;                           // m/s
const double MAX_REL_ROUGH = 0.05;   // Haaland/Moody range: e/D <= 0.05

enum class FlowRegime { Laminar, Transitional, Turbulent };

// ---- Data records ---------------------------------------------------------
struct Pipe  { double diameter; double length; double roughness; };
struct Fluid { double density;  double viscosity; };

struct Results {
    double area, velocity, flowRate;
    double reynolds, relRoughness, frictionFactor;
    double frictionFactorColebrook;   // independent iterative cross-check
    double pressureDrop, headLoss, gradient;
    FlowRegime regime;
};

// Typical absolute roughness table (Moody / Crane TP-410, White "Fluid Mechanics")
struct Material { const char* name; double roughness_m; };
extern const Material MATERIALS[];
extern const int NUM_MATERIALS;

// ---- Input validation (return empty string if OK, else error message) -----
std::string validatePipe(const Pipe& p);
std::string validateFluid(const Fluid& f);
std::string validateVelocity(double v);
std::string validateFlowRate(double q, double diameter);

// ---- Basic hydraulics -----------------------------------------------------
double crossSectionArea(double diameter);              // A = pi D^2 / 4
double velocityFromFlowRate(double q, double diameter);// V = Q / A
double flowRateFromVelocity(double v, double diameter);// Q = V A

// ---- Dimensionless numbers & friction ------------------------------------
double reynoldsNumber(double rho, double v, double d, double mu);
FlowRegime classifyRegime(double re);
std::string regimeName(FlowRegime r);
double frictionLaminar(double re);                     // f = 64/Re
double frictionHaaland(double re, double relRough);    // explicit approx.
double frictionColebrook(double re, double relRough);  // iterative (reference)
double frictionFactor(double re, double relRough);     // regime-aware selection

// ---- Darcy-Weisbach -------------------------------------------------------
double pressureDrop(double f, double length, double d, double rho, double v);
double headLoss(double dp, double rho);                // h = dP / (rho g)

// ---- Full analysis --------------------------------------------------------
Results analyseFromVelocity(const Pipe& p, const Fluid& f, double v);
Results analyseFromFlowRate(const Pipe& p, const Fluid& f, double q);

} // namespace pipeflow
#endif
