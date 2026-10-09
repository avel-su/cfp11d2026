#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <limits>

using namespace std;

const int MAX_RES = 20;          // maximum number of resistors
const double EPS = 1e-9;         // tolerance for floating-point comparison

enum class Topology { SERIES, PARALLEL };

// Data for one resistor
struct Resistor {
    double resistance;   // ohms
    double powerRating;  // watts
    double voltage;      // volts across it (calculated)
    double current;      // amps through it (calculated)
    double power;        // watts dissipated (calculated)
    bool   overloaded;   // true if power > powerRating
};

// Results for the whole circuit
struct CircuitResult {
    double totalResistance;
    double totalCurrent;
    double totalPower;
};

// ---------------------------------------------------------------- input ----

// Read a positive double (> 0). Re-prompts until valid.
double readPositiveDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0.0 && isfinite(value)) {
            return value;
        }
        cout << "  Invalid input. Please enter a number greater than 0.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Read an integer within [lo, hi]. Re-prompts until valid.
int readIntInRange(const string& prompt, int lo, int hi) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= lo && value <= hi) {
            return value;
        }
        cout << "  Invalid input. Please enter a whole number from "
             << lo << " to " << hi << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

Topology readTopology() {
    cout << "Circuit topology:\n  1) Series\n  2) Parallel\n";
    int choice = readIntInRange("Choose 1 or 2: ", 1, 2);
    return (choice == 1) ? Topology::SERIES : Topology::PARALLEL;
}

// ----------------------------------------------------------- calculation ----

double seriesResistance(const Resistor r[], int n) {
    double total = 0.0;
    for (int i = 0; i < n; i++) total += r[i].resistance;
    return total;
}

double parallelResistance(const Resistor r[], int n) {
    double sumInverse = 0.0;
    for (int i = 0; i < n; i++) sumInverse += 1.0 / r[i].resistance;
    return 1.0 / sumInverse;
}

// Fills in voltage, current, power and overload flag for every resistor
CircuitResult analyzeCircuit(Topology topo, double sourceVoltage,
                             Resistor r[], int n) {
    CircuitResult res;

    if (topo == Topology::SERIES) {
        res.totalResistance = seriesResistance(r, n);
        res.totalCurrent = sourceVoltage / res.totalResistance;
        for (int i = 0; i < n; i++) {
            r[i].current = res.totalCurrent;               // same current
            r[i].voltage = r[i].current * r[i].resistance; // V = I*R
        }
    } else {
        res.totalResistance = parallelResistance(r, n);
        res.totalCurrent = 0.0;
        for (int i = 0; i < n; i++) {
            r[i].voltage = sourceVoltage;                  // same voltage
            r[i].current = r[i].voltage / r[i].resistance; // I = V/R
            res.totalCurrent += r[i].current;              // KCL
        }
    }

    res.totalPower = 0.0;
    for (int i = 0; i < n; i++) {
        r[i].power = r[i].voltage * r[i].current;          // P = V*I
        r[i].overloaded = (r[i].power > r[i].powerRating + EPS);
        res.totalPower += r[i].power;
    }
    return res;
}

// Verification: P_source = V*I_total must equal the sum of resistor powers,
// and for series circuits the voltage drops must sum to the source voltage.
bool verifyResults(Topology topo, double sourceVoltage,
                   const Resistor r[], int n, const CircuitResult& res) {
    double sourcePower = sourceVoltage * res.totalCurrent;
    bool powerOk = fabs(sourcePower - res.totalPower) <= 1e-6 * max(1.0, sourcePower);

    bool kvlOk = true;
    if (topo == Topology::SERIES) {
        double sumV = 0.0;
        for (int i = 0; i < n; i++) sumV += r[i].voltage;
        kvlOk = fabs(sumV - sourceVoltage) <= 1e-6 * max(1.0, sourceVoltage);
    }
    return powerOk && kvlOk;
}

// ---------------------------------------------------------------- report ----

void printReport(Topology topo, double sourceVoltage,
                 const Resistor r[], int n, const CircuitResult& res) {
    cout << "\n==================== CIRCUIT ANALYSIS REPORT ====================\n";
    cout << "Topology       : " << (topo == Topology::SERIES ? "Series" : "Parallel") << "\n";
    cout << "Source voltage : " << fixed << setprecision(3) << sourceVoltage << " V\n";
    cout << "Resistors      : " << n << "\n\n";

    cout << left  << setw(6)  << "R#"
         << right << setw(12) << "R (ohm)"
         << setw(12) << "V (V)"
         << setw(12) << "I (A)"
         << setw(12) << "P (W)"
         << setw(12) << "Rating (W)"
         << setw(10) << "Status" << "\n";
    cout << string(76, '-') << "\n";

    for (int i = 0; i < n; i++) {
        cout << left  << setw(6) << ("R" + to_string(i + 1))
             << right << fixed << setprecision(3)
             << setw(12) << r[i].resistance
             << setw(12) << r[i].voltage
             << setprecision(4)
             << setw(12) << r[i].current
             << setw(12) << r[i].power
             << setprecision(3)
             << setw(12) << r[i].powerRating
             << setw(10) << (r[i].overloaded ? "OVERLOAD" : "OK") << "\n";
    }
    cout << string(76, '-') << "\n\n";

    cout << "Total resistance : " << setprecision(3) << res.totalResistance << " ohm\n";
    cout << "Total current    : " << setprecision(4) << res.totalCurrent << " A\n";
    cout << "Total power      : " << setprecision(4) << res.totalPower << " W\n\n";

    bool anyOverload = false;
    for (int i = 0; i < n; i++) {
        if (r[i].overloaded) {
            anyOverload = true;
            cout << "WARNING: R" << (i + 1) << " dissipates " << setprecision(4)
                 << r[i].power << " W, exceeding its " << setprecision(3)
                 << r[i].powerRating << " W rating ("
                 << setprecision(1) << (r[i].power / r[i].powerRating * 100.0)
                 << "% of rating).\n";
        }
    }
    if (!anyOverload) cout << "All resistors are within their power ratings.\n";

    cout << "Self-check (KVL / power balance): "
         << (verifyResults(topo, sourceVoltage, r, n, res) ? "PASSED" : "FAILED") << "\n";
    cout << "=================================================================\n";
}

// --------------------------------------------------------------- testing ----

// Built-in tests with hand-calculated answers. Run with:  ./circuit_analyzer --test
bool approx(double a, double b) { return fabs(a - b) < 1e-6; }

int runSelfTests() {
    int passed = 0, total = 0;
    auto check = [&](const string& name, bool ok) {
        total++;
        if (ok) passed++;
        cout << (ok ? "[PASS] " : "[FAIL] ") << name << "\n";
    };

    // Test 1: series 100 + 200 + 300 ohm, 12 V -> Rt = 600, I = 0.02 A
    {
        Resistor r[3] = {{100, 0.25, 0, 0, 0, false},
                         {200, 0.25, 0, 0, 0, false},
                         {300, 0.25, 0, 0, 0, false}};
        CircuitResult res = analyzeCircuit(Topology::SERIES, 12.0, r, 3);
        check("Series: total resistance = 600", approx(res.totalResistance, 600));
        check("Series: current = 0.02 A", approx(res.totalCurrent, 0.02));
        check("Series: V3 = 6 V", approx(r[2].voltage, 6.0));
        check("Series: P3 = 0.12 W", approx(r[2].power, 0.12));
        check("Series: total power = 0.24 W", approx(res.totalPower, 0.24));
        check("Series: self-verification", verifyResults(Topology::SERIES, 12.0, r, 3, res));
    }
    // Test 2: parallel 100 || 100, 10 V -> Rt = 50, I1 = I2 = 0.1, It = 0.2
    {
        Resistor r[2] = {{100, 1.0, 0, 0, 0, false},
                         {100, 1.0, 0, 0, 0, false}};
        CircuitResult res = analyzeCircuit(Topology::PARALLEL, 10.0, r, 2);
        check("Parallel: total resistance = 50", approx(res.totalResistance, 50));
        check("Parallel: branch current = 0.1 A", approx(r[0].current, 0.1));
        check("Parallel: total current = 0.2 A", approx(res.totalCurrent, 0.2));
        check("Parallel: total power = 2 W", approx(res.totalPower, 2.0));
        check("Parallel: self-verification", verifyResults(Topology::PARALLEL, 10.0, r, 2, res));
    }
    // Test 3: overload detection. 10 ohm across 10 V = 10 W, rating 0.25 W
    {
        Resistor r[1] = {{10, 0.25, 0, 0, 0, false}};
        analyzeCircuit(Topology::SERIES, 10.0, r, 1);
        check("Overload detected (10 W > 0.25 W)", r[0].overloaded);
    }
    // Test 4: boundary - power exactly at rating is NOT an overload
    {
        Resistor r[1] = {{100, 1.0, 0, 0, 0, false}};  // 10 V -> exactly 1 W
        analyzeCircuit(Topology::SERIES, 10.0, r, 1);
        check("Boundary: P == rating is not overload", !r[0].overloaded);
    }
    // Test 5: very different resistances in parallel (1 ohm || 1 Mohm ~ 1 ohm)
    {
        Resistor r[2] = {{1.0, 5.0, 0, 0, 0, false},
                         {1e6, 5.0, 0, 0, 0, false}};
        CircuitResult res = analyzeCircuit(Topology::PARALLEL, 5.0, r, 2);
        check("Parallel: dominated by small R (~1 ohm)", fabs(res.totalResistance - 1.0) < 1e-5);
    }

    cout << "\n" << passed << " / " << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}

// ------------------------------------------------------------------ main ----

int main(int argc, char* argv[]) {
    if (argc > 1 && string(argv[1]) == "--test") {
        return runSelfTests();
    }

    cout << "==== DC Resistor Circuit Analyzer ====\n"
         << "Units: volts (V), ohms (ohm), watts (W)\n\n";

    char again = 'y';
    while (again == 'y' || again == 'Y') {
        Topology topo = readTopology();
        double sourceVoltage = readPositiveDouble("Source voltage (V): ");
        int n = readIntInRange("Number of resistors (1-" + to_string(MAX_RES) + "): ",
                               1, MAX_RES);

        Resistor resistors[MAX_RES];
        for (int i = 0; i < n; i++) {
            cout << "\n-- Resistor R" << (i + 1) << " --\n";
            resistors[i].resistance  = readPositiveDouble("  Resistance (ohm): ");
            resistors[i].powerRating = readPositiveDouble("  Power rating (W, e.g. 0.25): ");
            resistors[i].voltage = resistors[i].current = resistors[i].power = 0.0;
            resistors[i].overloaded = false;
        }

        CircuitResult result = analyzeCircuit(topo, sourceVoltage, resistors, n);
        printReport(topo, sourceVoltage, resistors, n, result);

        cout << "\nAnalyze another circuit? (y/n): ";
        cin >> again;
    }
    cout << "Goodbye!\n";
    return 0;
}