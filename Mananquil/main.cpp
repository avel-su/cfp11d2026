// main.cpp
// Pipe Flow & Pressure Drop Calculator - user interface
// Build: g++ -std=c++17 -Wall -Wextra -o pipeflow main.cpp calculations.cpp
#include "calculations.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;
using namespace pipeflow;

// ---- Robust input helpers --------------------------------------------------
// Reads a number; repeats until the user types a valid number (no letters).
double readNumber(const string& prompt) {
    double x;
    while (true) {
        cout << prompt;
        if (cin >> x) return x;
        if (cin.eof()) { cout << "\nInput ended. Exiting.\n"; exit(1); }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Error: please enter a numeric value.\n";
    }
}

int readChoice(const string& prompt, int lo, int hi) {
    while (true) {
        double x = readNumber(prompt);
        int n = static_cast<int>(x);
        if (x == n && n >= lo && n <= hi) return n;
        cout << "  Error: choose a whole number from " << lo << " to " << hi << ".\n";
    }
}

// ---- Input sections ---------------------------------------------------------
Pipe readPipe() {
    Pipe p;
    while (true) {
        cout << "\n--- PIPE PROPERTIES ---\n";
        p.diameter = readNumber("Inside diameter D [mm]: ") / 1000.0;
        p.length   = readNumber("Pipe length L [m]: ");

        cout << "Pipe material (absolute roughness e):\n";
        for (int i = 0; i < NUM_MATERIALS; ++i)
            cout << "  " << (i + 1) << ") " << left << setw(30) << MATERIALS[i].name
                 << right << fixed << setprecision(4)
                 << MATERIALS[i].roughness_m * 1000.0 << " mm\n";
        cout << "  " << (NUM_MATERIALS + 1) << ") Custom roughness\n";
        int c = readChoice("Select material: ", 1, NUM_MATERIALS + 1);
        p.roughness = (c <= NUM_MATERIALS) ? MATERIALS[c - 1].roughness_m
                                           : readNumber("Roughness e [mm]: ") / 1000.0;

        string err = validatePipe(p);
        if (err.empty()) return p;
        cout << "  INPUT ERROR: " << err << "\n  Please re-enter the pipe data.\n";
    }
}

Fluid readFluid() {
    Fluid f;
    while (true) {
        cout << "\n--- FLUID PROPERTIES ---\n"
             << "  1) Water at 20 C   (rho = 998.2, mu = 1.002e-3)\n"
             << "  2) Air at 20 C     (rho = 1.204, mu = 1.825e-5)\n"
             << "  3) SAE 30 oil      (rho = 891,   mu = 0.29)\n"
             << "  4) Custom fluid\n";
        int c = readChoice("Select fluid: ", 1, 4);
        switch (c) {
            case 1: f = {998.2, 1.002e-3}; break;
            case 2: f = {1.204, 1.825e-5}; break;
            case 3: f = {891.0, 0.29};     break;
            default:
                f.density   = readNumber("Density rho [kg/m^3]: ");
                f.viscosity = readNumber("Dynamic viscosity mu [Pa.s]: ");
        }
        string err = validateFluid(f);
        if (err.empty()) return f;
        cout << "  INPUT ERROR: " << err << "\n  Please re-enter the fluid data.\n";
    }
}

Results readFlowAndAnalyse(const Pipe& p, const Fluid& f) {
    cout << "\n--- FLOW CONDITION ---\n  1) Known flow rate Q [L/s]\n  2) Known velocity V [m/s]\n";
    int mode = readChoice("Select: ", 1, 2);
    while (true) {
        if (mode == 1) {
            double q = readNumber("Flow rate Q [L/s]: ") / 1000.0;
            string err = validateFlowRate(q, p.diameter);
            if (err.empty()) return analyseFromFlowRate(p, f, q);
            cout << "  INPUT ERROR: " << err << "\n";
        } else {
            double v = readNumber("Velocity V [m/s]: ");
            string err = validateVelocity(v);
            if (err.empty()) return analyseFromVelocity(p, f, v);
            cout << "  INPUT ERROR: " << err << "\n";
        }
    }
}

// ---- Report -----------------------------------------------------------------
void printReport(const Pipe& p, const Fluid& f, const Results& r) {
    const string line(60, '=');
    cout << "\n" << line << "\n       PIPE FLOW & PRESSURE DROP REPORT\n" << line << "\n";
    cout << fixed;
    cout << "INPUT DATA\n"
         << setprecision(2) << "  Diameter            : " << p.diameter * 1000 << " mm\n"
         << setprecision(2) << "  Length              : " << p.length << " m\n"
         << setprecision(4) << "  Roughness e         : " << p.roughness * 1000 << " mm\n"
         << setprecision(1) << "  Density             : " << f.density << " kg/m^3\n"
         << scientific << setprecision(3)
         << "  Dynamic viscosity   : " << f.viscosity << " Pa.s\n" << fixed;
    cout << "\nFLOW\n"
         << setprecision(5) << "  Area                : " << r.area << " m^2\n"
         << setprecision(3) << "  Velocity            : " << r.velocity << " m/s\n"
         << setprecision(3) << "  Discharge Q         : " << r.flowRate * 1000 << " L/s  ("
         << setprecision(5) << r.flowRate << " m^3/s)\n";
    cout << "\nREGIME\n"
         << setprecision(0) << "  Reynolds number     : " << r.reynolds << "\n"
         << "  Flow regime         : " << regimeName(r.regime) << "\n";
    cout << "\nFRICTION\n"
         << setprecision(6) << "  Relative roughness  : " << r.relRoughness << "\n"
         << setprecision(5) << "  Friction factor f   : " << r.frictionFactor
         << (r.regime == FlowRegime::Laminar ? "  (64/Re)\n" : "  (Haaland)\n");
    if (r.regime != FlowRegime::Laminar)
        cout << "  Colebrook check     : " << r.frictionFactorColebrook << "  (diff "
             << setprecision(2) << 100.0 * (r.frictionFactor - r.frictionFactorColebrook)
                                         / r.frictionFactorColebrook << " %)\n";
    cout << "\nDARCY-WEISBACH RESULTS\n"
         << setprecision(2) << "  Pressure drop       : " << r.pressureDrop << " Pa  = "
         << r.pressureDrop / 1000.0 << " kPa\n"
         << setprecision(3) << "  Head loss           : " << r.headLoss << " m of fluid\n"
         << setprecision(2) << "  Gradient dP/L       : " << r.gradient << " Pa/m\n";
    if (r.regime == FlowRegime::Transitional)
        cout << "\n  WARNING: 2300 < Re < 4000. Flow is unstable; friction factor and\n"
                "  pressure drop are estimates only. Treat results with caution.\n";
    cout << "\nNote: straight horizontal pipe, minor losses not included.\n" << line << "\n";
}

int main() {
    cout << "PIPE FLOW & PRESSURE DROP CALCULATOR (SI units)\n";
    do {
        Pipe  pipe  = readPipe();
        Fluid fluid = readFluid();
        Results res = readFlowAndAnalyse(pipe, fluid);
        printReport(pipe, fluid, res);
    } while (readChoice("\nRun another case? (1 = yes, 2 = no): ", 1, 2) == 1);
    cout << "Done.\n";
    return 0;
}
