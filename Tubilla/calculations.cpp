#include "calculations.h"
#include <cmath>

// Calculate optimal cycle length using Webster's formula
double calcCycleLength(Lane lanes[], int numLanes, double maxWaitTime) {
    double lostTime = numLanes * 4.0;
    double sumY = 0.0;
    
    for (int i = 0; i < numLanes; i++) {
        sumY += lanes[i].arrivalRate / lanes[i].saturationFlow;
    }
    
    double cycle = (1.5 * lostTime + 5.0) / (1.0 - sumY);
    
    if (cycle < 30.0) cycle = 30.0;
    if (cycle > maxWaitTime * 2.0) cycle = maxWaitTime * 2.0;
    return cycle;
}

// Distribute green time proportionally
void assignGreenTime(Lane lanes[], int numLanes, double cycleLength) {
    double lostTime = numLanes * 4.0;
    double availableTime = cycleLength - lostTime;
    double totalFlow = 0.0;
    
    for (int i = 0; i < numLanes; i++)
        totalFlow += lanes[i].arrivalRate;
    
    for (int i = 0; i < numLanes; i++) {
        double ratio = lanes[i].arrivalRate / totalFlow;
        lanes[i].greenTime = static_cast<int>(availableTime * ratio);
    }
}

// Calculate average delay per vehicle
double calcDelay(double arrivalRate, double saturationFlow, double greenTime, double cycleLength) {
    double v = arrivalRate / 3600.0;
    double s = saturationFlow / 3600.0;
    double gC = greenTime / cycleLength;
    
    if (v >= s * gC) return 999.0;
    
    double uniformDelay = (cycleLength * (1 - gC) * (1 - gC)) / (2 * (1 - v/s));
    double randomDelay = (v * v) / (2 * s * gC * (s * gC - v));
    
    return uniformDelay + randomDelay;
}

// Calculate max queue length during red
double calcQueue(double arrivalRate, double cycleLength, double greenTime) {
    double redTime = cycleLength - greenTime;
    return (arrivalRate / 3600.0) * redTime;
}
