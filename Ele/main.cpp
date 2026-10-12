// ==========================================
//   BEAM LOAD & DEFLECTION CALCULATOR
//   Student: Aaron T Ele
//   Email: aarontele@su.edu.ph
//   Date: October 2026
// ==========================================

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

// Calculate reactions at supports
void calculateReactions(double L, double P, double a, double w) {
    double R1 = (P * (L - a) / L) + (w * L / 2.0);
    double R2 = (P * a / L) + (w * L / 2.0);

    cout << fixed << setprecision(2);
    cout << "\n--- SUPPORT REACTIONS ---" << endl;
    cout << "Reaction at left support R1: " << R1 << " kN" << endl;
    cout << "Reaction at right support R2: " << R2 << " kN" << endl;
    cout << "Check (sum should equal total load): " << R1 + R2 << " kN\n" << endl;
}

// Calculate maximum bending moment
double maxBendingMoment(double L, double P, double a, double w) {
    double b = L - a;
    double M_point = P * a * b / L;
    double M_udl = w * L * L / 8.0;
    return M_point + M_udl;
}

// Calculate maximum deflection
double maxDeflection(double L, double P, double a, double w, double E, double I) {
    double b = L - a;
    // Convert E from GPa to Pa, I stays in m⁴
    double E_pa = E * 1e9;
    double L_m = L;

    // Deflection at center for point load
    double delta_point = (P * a * b / (6.0 * E_pa * I * L_m)) * sqrt(a * (L_m + b) * b * (L_m + a));
    // Simpler formula: max deflection for UDL
    double delta_udl = 5.0 * w * pow(L_m, 4) / (384.0 * E_pa * I);

    return delta_point + delta_udl;
}

// Validate inputs
bool validateInputs(double L, double P, double a, double w, double E, double I) {
    if (L <= 0 || P < 0 || a < 0 || a > L || w < 0 || E <= 0 || I <= 0) {
        cout << "ERROR: Check your inputs!" << endl;
        if (L <= 0) cout << "- Length must be positive" << endl;
        if (a < 0 || a > L) cout << "- Point load position must be between 0 and length" << endl;
        if (E <= 0 || I <= 0) cout << "- Modulus E and Moment of Inertia I must be positive" << endl;
        return false;
    }
    return true;
}

// Display full report
void displayReport(double L, double P, double a, double w, double E, double I) {
    cout << fixed << setprecision(2);
    cout << "\n========================================" << endl;
    cout << "       BEAM DESIGN REPORT" << endl;
    cout << "========================================" << endl;
    cout << "Beam Span:         " << L << " m" << endl;
    cout << "Point Load:        " << P << " kN at " << a << " m from left" << endl;
    cout << "Uniform Load:      " << w << " kN/m" << endl;
    cout << "Modulus E:         " << E << " GPa" << endl;
    cout << "Inertia I:         " << I << " m⁴" << endl;

    calculateReactions(L, P, a, w);

    double M_max = maxBendingMoment(L, P, a, w);
    cout << "MAX BENDING MOMENT: " << M_max << " kN·m" << endl;

    double delta_max = maxDeflection(L, P, a, w, E, I);
    cout << "MAX DEFLECTION:     " << delta_max * 1000 << " mm\n" << endl;

    double allowable = L / 360.0;
    if (delta_max <= allowable) {
        cout << "✓ Deflection within limit (L/360 = " << allowable * 1000 << " mm)" << endl;
    } else {
        cout << "⚠ Deflection EXCEEDS limit! Allowable: " << allowable * 1000 << " mm" << endl;
        cout << "  Stiffen beam or reduce load." << endl;
    }
    cout << "========================================\n" << endl;
}

int main() {
    cout << "BEAM LOAD & DEFLECTION CALCULATOR" << endl;
    cout << "Enter values below:\n" << endl;

    double L, P, a, w, E, I;

    cout << "Beam span L (meters): ";
    cin >> L;

    cout << "Point load P (kN): ";
    cin >> P;

    cout << "Distance from left to point load a (m): ";
    cin >> a;

    cout << "Uniform distributed load w (kN/m): ";
    cin >> w;

    cout << "Modulus of Elasticity E (GPa) [Steel=200]: ";
    cin >> E;

    cout << "Moment of Inertia I (m⁴): ";
    cin >> I;

    if (!validateInputs(L, P, a, w, E, I)) {
        return 1;
    }

    displayReport(L, P, a, w, E, I);

    return 0;
}
