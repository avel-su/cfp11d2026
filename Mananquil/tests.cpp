// tests.cpp - verification of calculation module against hand calculations,
// reference values, boundary cases and invalid inputs.
// Build: g++ -std=c++17 -Wall -Wextra -o tests tests.cpp calculations.cpp && ./tests
#include "calculations.h"
#include <iostream>
#include <cmath>
using namespace pipeflow;

static int passed = 0, failed = 0;
void checkNear(const char* name, double got, double expect, double relTol) {
    bool ok = std::fabs(got - expect) <= relTol * std::fabs(expect);
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << "  got=" << got << "  expected=" << expect << "\n";
    ok ? ++passed : ++failed;
}
void checkTrue(const char* name, bool cond) {
    std::cout << (cond ? "[PASS] " : "[FAIL] ") << name << "\n";
    cond ? ++passed : ++failed;
}

int main() {
    const Fluid water{998.2, 1.002e-3};

    // 1. Basic hydraulics: D = 0.1 m -> A = 7.854e-3 m^2; Q = 2 * A = 0.015708 m^3/s
    checkNear("Area D=0.1 m", crossSectionArea(0.1), 7.853982e-3, 1e-6);
    checkNear("Q from V=2 m/s", flowRateFromVelocity(2.0, 0.1), 0.01570796, 1e-6);
    checkNear("V from Q round trip", velocityFromFlowRate(0.01570796, 0.1), 2.0, 1e-6);

    // 2. Reynolds: 998.2*2*0.1/1.002e-3 = 199,242 (hand calc)
    checkNear("Reynolds water", reynoldsNumber(998.2, 2.0, 0.1, 1.002e-3), 199242.5, 1e-4);

    // 3. Regime boundaries
    checkTrue("Re=1000 laminar",      classifyRegime(1000)  == FlowRegime::Laminar);
    checkTrue("Re=2300 transitional", classifyRegime(2300)  == FlowRegime::Transitional);
    checkTrue("Re=3000 transitional", classifyRegime(3000)  == FlowRegime::Transitional);
    checkTrue("Re=4001 turbulent",    classifyRegime(4001)  == FlowRegime::Turbulent);

    // 4. Laminar: oil, D=0.05, L=10, V=0.5. Hagen-Poiseuille: dP = 32 mu L V / D^2 = 6400 Pa
    Pipe oilPipe{0.05, 10.0, 0.000045};
    Fluid oil{900.0, 0.1};
    Results lam = analyseFromVelocity(oilPipe, oil, 0.5);
    checkNear("Laminar Re", lam.reynolds, 225.0, 1e-9);
    checkNear("Laminar f=64/Re", lam.frictionFactor, 64.0 / 225.0, 1e-9);
    checkNear("Laminar dP = Hagen-Poiseuille", lam.pressureDrop, 6400.0, 1e-9);

    // 5. Turbulent: water, D=0.1, L=100, V=2, commercial steel e=0.045 mm
    //    hand calc: Haaland f = 0.01838, dP = 36.7 kPa
    Pipe steel{0.1, 100.0, 0.000045};
    Results t = analyseFromVelocity(steel, water, 2.0);
    checkNear("Turbulent f (Haaland, hand calc)", t.frictionFactor, 0.01838, 2e-3);
    checkNear("Turbulent dP (hand calc)", t.pressureDrop, 36700.0, 5e-3);
    checkNear("Head loss = dP/(rho g)", t.headLoss, t.pressureDrop / (998.2 * GRAVITY), 1e-9);

    // 6. Haaland vs Colebrook agree within ~2% over a range of cases
    double res[] = {5e3, 1e4, 1e5, 1e6, 1e7};
    double rr[]  = {1e-6, 1e-4, 1e-3, 1e-2, 0.04};
    double worst = 0;
    for (double re : res) for (double e : rr)
        worst = std::fmax(worst, std::fabs(frictionHaaland(re, e) - frictionColebrook(re, e)) / frictionColebrook(re, e));
    checkTrue("Haaland within 2.5% of Colebrook (25 cases)", worst < 0.025);

    // 7. Reference: Moody chart, Re=1e5, e/D=0 -> smooth pipe f ~ 0.0180 (Blasius 0.316/Re^0.25 = 0.0178)
    checkNear("Smooth pipe Re=1e5 vs Blasius", frictionHaaland(1e5, 0.0), 0.0178, 0.04);
    // Fully rough limit: e/D=0.01, Re=1e7 -> f ~ 0.038 (Moody chart)
    checkNear("Fully rough e/D=0.01", frictionColebrook(1e7, 0.01), 0.0380, 0.02);

    // 8. Boundary values
    Pipe minPipe{MIN_DIAMETER, 1.0, 0.0};
    checkTrue("Min diameter accepted", validatePipe(minPipe).empty());
    Pipe maxPipe{MAX_DIAMETER, MAX_LENGTH, 0.0};
    checkTrue("Max diameter/length accepted", validatePipe(maxPipe).empty());
    Results tiny = analyseFromVelocity(minPipe, water, 0.001);   // very low flow
    checkTrue("Tiny flow laminar & finite dP", tiny.regime == FlowRegime::Laminar && std::isfinite(tiny.pressureDrop));
    Results big = analyseFromVelocity(maxPipe, water, MAX_VELOCITY);
    checkTrue("Max flow finite dP", std::isfinite(big.pressureDrop) && big.pressureDrop > 0);
    checkTrue("Smooth pipe (e=0) works", std::isfinite(analyseFromVelocity(Pipe{0.1,10,0}, water, 2).frictionFactor));

    // 9. Invalid inputs must be rejected
    checkTrue("Zero diameter rejected",       !validatePipe({0.0, 10, 0}).empty());
    checkTrue("Negative diameter rejected",   !validatePipe({-0.1, 10, 0}).empty());
    checkTrue("Zero length rejected",         !validatePipe({0.1, 0, 0}).empty());
    checkTrue("Negative roughness rejected",  !validatePipe({0.1, 10, -1e-5}).empty());
    checkTrue("e/D > 0.05 rejected",          !validatePipe({0.1, 10, 0.01}).empty());
    checkTrue("NaN diameter rejected",        !validatePipe({NAN, 10, 0}).empty());
    checkTrue("Zero density rejected",        !validateFluid({0.0, 1e-3}).empty());
    checkTrue("Zero viscosity rejected",      !validateFluid({1000, 0.0}).empty());
    checkTrue("Negative viscosity rejected",  !validateFluid({1000, -1e-3}).empty());
    checkTrue("Zero velocity rejected",       !validateVelocity(0).empty());
    checkTrue("Excess velocity rejected",     !validateVelocity(500).empty());
    checkTrue("Negative flow rejected",       !validateFlowRate(-1, 0.1).empty());

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
