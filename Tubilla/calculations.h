#ifndef CALCULATIONS_H
#define CALCULATIONS_H

struct Lane {
    char direction[20];
    double arrivalRate;     // vehicles/hour
    double saturationFlow;  // vehicles/hour
    int greenTime;          // seconds
};

double calcCycleLength(Lane lanes[], int numLanes, double maxWaitTime);
void assignGreenTime(Lane lanes[], int numLanes, double cycleLength);
double calcDelay(double arrivalRate, double saturationFlow, double greenTime, double cycleLength);
double calcQueue(double arrivalRate, double cycleLength, double greenTime);

#endif
