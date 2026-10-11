// calculations.cpp
// Implementation of pipe-flow calculations.
//
// ASSUMPTIONS & LIMITATIONS
//  - Steady, fully developed, incompressible flow of a Newtonian fluid.
//  - Full circular pipe, constant diameter, straight and horizontal
//    (no elevation change). Minor losses (bends, valves) are NOT included.
//  - Constant fluid properties (no temperature variation along the pipe).
//  - Haaland valid for Re >= 4000 and 1e-6 <= e/D <= 0.05 (about +/-2% of Colebrook).
//  - Transitional flow (2300 < Re < 4000) is unstable; the result is an
//    ESTIMATE only (turbulent correlation is used) and the program warns the user.
//
// REFERENCES
//  - Darcy-Weisbach equation; Hagen-Poiseuille (f = 64/Re) for laminar flow.
//  - Haaland, S.E. (1983), J. Fluids Eng. 105(1):89-90.
//  - Colebrook, C.F. (1939), J. Inst. Civil Eng. 11:133-156.
//  - Moody, L.F. (1944), Trans. ASME 66:671-684 (roughness values).
//  - White, F.M., Fluid Mechanics; Crane Technical Paper No. 410.
#include "calculations.h"
#include <cmath>

namespace pipeflow {

const Material MATERIALS[] = {
    {"PVC / plastic / drawn tubing", 0.0000015},
    {"Commercial / welded steel",    0.000045},
    {"Wrought iron",                 0.000045},
    {"Galvanized iron",              0.00015},
    {"Asphalted cast iron",          0.00012},
    {"Cast iron",                    0.00026},
    {"Concrete (typical)",           0.001},
    {"Riveted steel (typical)",      0.003},
};
const int NUM_MATERIALS = sizeof(MATERIALS) / sizeof(MATERIALS[0]);

// ---------------------------------------------------------------------------
// Validation
// ---------------------------------------------------------------------------
std::string validatePipe(const Pipe& p) {
    if (!std::isfinite(p.diameter) || p.diameter < MIN_DIAMETER || p.diameter > MAX_DIAMETER)
        return "Diameter must be between 0.001 m and 5 m.";
    if (!std::isfinite(p.length) || p.length <= 0.0 || p.length > MAX_LENGTH)
        return "Length must be greater than 0 and not more than 100000 m.";
    if (!std::isfinite(p.roughness) || p.roughness < 0.0)
        return "Roughness cannot be negative.";
    if (p.roughness / p.diameter > MAX_REL_ROUGH)
        return "Relative roughness e/D exceeds 0.05 (outside Moody/Haaland range); "
               "check that roughness and diameter are in metres.";
    return "";
}

std::string validateFluid(const Fluid& f) {
    if (!std::isfinite(f.density) || f.density < MIN_DENSITY || f.density > MAX_DENSITY)
        return "Density must be between 0.1 and 25000 kg/m^3.";
    if (!std::isfinite(f.viscosity) || f.viscosity < MIN_VISCOSITY || f.viscosity > MAX_VISCOSITY)
        return "Dynamic viscosity must be between 1e-6 and 10 Pa.s.";
    return "";
}

std::string validateVelocity(double v) {
    if (!std::isfinite(v) || v <= 0.0) return "Velocity must be greater than 0.";
    if (v > MAX_VELOCITY)              return "Velocity above 100 m/s is outside the incompressible range.";
    return "";
}

std::string validateFlowRate(double q, double diameter) {
    if (!std::isfinite(q) || q <= 0.0) return "Flow rate must be greater than 0.";
    // Reuse the velocity limit through the implied velocity.
    return validateVelocity(velocityFromFlowRate(q, diameter));
}

// ---------------------------------------------------------------------------
// Basic hydraulics
// ---------------------------------------------------------------------------
double crossSectionArea(double d)               { return PI * d * d / 4.0; }           // m^2
double velocityFromFlowRate(double q, double d) { return q / crossSectionArea(d); }    // m/s
double flowRateFromVelocity(double v, double d) { return v * crossSectionArea(d); }    // m^3/s

// ---------------------------------------------------------------------------
// Reynolds number and regime
//   Re = rho * V * D / mu   (inertial forces / viscous forces)
// ---------------------------------------------------------------------------
double reynoldsNumber(double rho, double v, double d, double mu) {
    return rho * v * d / mu;
}

FlowRegime classifyRegime(double re) {
    if (re < RE_LAMINAR)   return FlowRegime::Laminar;
    if (re <= RE_TURBULENT) return FlowRegime::Transitional;
    return FlowRegime::Turbulent;
}

std::string regimeName(FlowRegime r) {
    switch (r) {
        case FlowRegime::Laminar:      return "LAMINAR";
        case FlowRegime::Transitional: return "TRANSITIONAL (unstable - estimate only)";
        default:                       return "TURBULENT";
    }
}

// ---------------------------------------------------------------------------
// Friction factor (Darcy)
// ---------------------------------------------------------------------------
// Laminar (Hagen-Poiseuille): f = 64 / Re   (independent of roughness)
double frictionLaminar(double re) { return 64.0 / re; }

// Haaland (1983) explicit approximation to Colebrook:
//   1/sqrt(f) = -1.8 * log10[ (e/D / 3.7)^1.11 + 6.9/Re ]
double frictionHaaland(double re, double relRough) {
    double term  = std::pow(relRough / 3.7, 1.11) + 6.9 / re;
    double invSq = -1.8 * std::log10(term);
    return 1.0 / (invSq * invSq);
}

// Colebrook-White (implicit), solved by fixed-point iteration:
//   1/sqrt(f) = -2 * log10[ (e/D)/3.7 + 2.51 / (Re * sqrt(f)) ]
// Starts from the Haaland value and converges in a few iterations.
// Used as an independent cross-check of Haaland.
double frictionColebrook(double re, double relRough) {
    double f = frictionHaaland(re, relRough);        // good initial guess
    for (int i = 0; i < 100; ++i) {
        double rhs = -2.0 * std::log10(relRough / 3.7 + 2.51 / (re * std::sqrt(f)));
        double fNew = 1.0 / (rhs * rhs);
        if (std::fabs(fNew - f) < 1.0e-10) return fNew;   // converged
        f = fNew;
    }
    return f;
}

// Regime-aware friction factor: laminar -> 64/Re, otherwise Haaland.
double frictionFactor(double re, double relRough) {
    if (classifyRegime(re) == FlowRegime::Laminar) return frictionLaminar(re);
    return frictionHaaland(re, relRough);
}

// ---------------------------------------------------------------------------
// Darcy-Weisbach:  dP = f * (L/D) * (rho * V^2 / 2)      [Pa]
// Head loss:       h  = dP / (rho * g)                    [m of fluid]
// ---------------------------------------------------------------------------
double pressureDrop(double f, double length, double d, double rho, double v) {
    return f * (length / d) * (rho * v * v / 2.0);
}
double headLoss(double dp, double rho) { return dp / (rho * GRAVITY); }

// ---------------------------------------------------------------------------
// Full analysis
// ---------------------------------------------------------------------------
Results analyseFromVelocity(const Pipe& p, const Fluid& fl, double v) {
    Results r;
    r.velocity       = v;
    r.area           = crossSectionArea(p.diameter);
    r.flowRate       = flowRateFromVelocity(v, p.diameter);
    r.reynolds       = reynoldsNumber(fl.density, v, p.diameter, fl.viscosity);
    r.relRoughness   = p.roughness / p.diameter;
    r.regime         = classifyRegime(r.reynolds);
    r.frictionFactor = frictionFactor(r.reynolds, r.relRoughness);
    r.frictionFactorColebrook = (r.regime == FlowRegime::Laminar)
                                ? r.frictionFactor
                                : frictionColebrook(r.reynolds, r.relRoughness);
    r.pressureDrop   = pressureDrop(r.frictionFactor, p.length, p.diameter, fl.density, v);
    r.headLoss       = headLoss(r.pressureDrop, fl.density);
    r.gradient       = r.pressureDrop / p.length;
    return r;
}

Results analyseFromFlowRate(const Pipe& p, const Fluid& fl, double q) {
    return analyseFromVelocity(p, fl, velocityFromFlowRate(q, p.diameter));
}

} // namespace pipeflow
