#include <iostream>
#include <iomanip>
#include <cstring>
#include "calculations.h"

using namespace std;

bool validateInput(double value, double minVal, double maxVal) {
    return (value >= minVal && value <= maxVal);
}

int main() {
    cout << "========================================\n";
    cout << "    TRAFFIC SIGNAL TIMING OPTIMIZER\n";
    cout << "========================================\n\n";
    
    const int NUM_LANES = 4;
    Lane lanes[NUM_LANES];
    
    strcpy(lanes[0].direction, "Northbound");
    strcpy(lanes[1].direction, "Southbound");
    strcpy(lanes[2].direction, "Eastbound");
    strcpy(lanes[3].direction, "Westbound");
    
    for (int i = 0; i < NUM_LANES; i++) {
        lanes[i].saturationFlow = 1800.0;
        lanes[i].greenTime = 0;
    }
    
    double maxWaitTime;
    
    // INPUT: Arrival Rates
    cout << "--- VEHICLE ARRIVAL RATES (vehicles/hour) ---\n";
    for (int i = 0; i < NUM_LANES; i++) {
        cout << lanes[i].direction << ": ";
        cin >> lanes[i].arrivalRate;
        while (!validateInput(lanes[i].arrivalRate, 0.0, 2000.0)) {
            cout << "  Invalid! Enter 0-2000: ";
            cin >> lanes[i].arrivalRate;
        }
    }
    
    // INPUT: Max Wait Time
    cout << "\n--- SIGNAL CONSTRAINTS ---\n";
    cout << "Maximum Allowed Wait Time (seconds): ";
    cin >> maxWaitTime;
    while (!validateInput(maxWaitTime, 20.0, 120.0)) {
        cout << "  Invalid! Enter 20-120 seconds: ";
        cin >> maxWaitTime;
    }
    
    cout << "\nCalculating...\n";
    
    // CALCULATIONS
    double cycleLength = calcCycleLength(lanes, NUM_LANES, maxWaitTime);
    assignGreenTime(lanes, NUM_LANES, cycleLength);
    
    // DISPLAY RESULTS
    cout << "\n========================================\n";
    cout << "         SIGNAL TIMING REPORT\n";
    cout << "========================================\n\n";
    
    cout << "Optimal Cycle Length: " << static_cast<int>(cycleLength) << " seconds\n\n";
    
    cout << left << setw(15) << "Direction"
         << right << setw(12) << "Arrival"
         << setw(12) << "Green Time"
         << setw(14) << "Avg Delay"
         << setw(14) << "Max Queue" << endl;
    cout << string(68, '-') << endl;
    
    double totalDelay = 0.0;
    for (int i = 0; i < NUM_LANES; i++) {
        double delay = calcDelay(lanes[i].arrivalRate, lanes[i].saturationFlow, lanes[i].greenTime, cycleLength);
        double queue = calcQueue(lanes[i].arrivalRate, cycleLength, lanes[i].greenTime);
        totalDelay += delay;
        
        cout << left << setw(15) << lanes[i].direction
             << right << setw(10) << fixed << setprecision(1) << lanes[i].arrivalRate << " v/h"
             << setw(10) << lanes[i].greenTime << " s"
             << setw(12) << setprecision(2) << delay << " s"
             << setw(12) << setprecision(2) << queue << " veh" << endl;
    }
    
    cout << string(68, '-') << endl;
    cout << "\nAverage Intersection Delay: " << fixed << setprecision(2) 
         << (totalDelay / NUM_LANES) << " seconds/vehicle\n";
    cout << "========================================\n";
    cout << "         ANALYSIS COMPLETE\n";
    cout << "========================================\n";
    
    return 0;
}
