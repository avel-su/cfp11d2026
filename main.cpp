/**
 * Building Energy Consumption & Cost Estimator
 *
 * References & Standards:
 * - ASHRAE Handbook of Fundamentals (Heat Transfer Concepts & Degree-Day Approaches)
 * - ISO 13790 (Thermal performance of buildings - Energy use for space heating and cooling)
 *
 * Engineering Assumptions & Limitations:
 * 1. Heat transfer models conduct steady-state heat loss/gain through building envelope elements:
 *    Q_envelope = (A / R) * Delta_T [Watts]
 * 2. Infiltration/ventilation loads are approximated using the air change rate method:
 *    Q_ventilation = V * ACH * rho * C_p * Delta_T / 3600 [Watts]
 * 3. Annual energy estimation uses standard Heating Degree Days (HDD) and Cooling Degree Days (CDD).
 * 4. Latent heat gains, solar heat gain coefficient (SHGC) dynamics, and thermal mass capacitance
 *    are simplified to representative internal and solar heat gain peak allowances.
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <limits>
#include <vector>

// Constants
const double AIR_DENSITY = 1.204;       // kg/m^3 (at ~20 °C, 1 atm)
const double AIR_SPECIFIC_HEAT = 1005.0; // J/(kg·K)
const double CLEAR_HEIGHT = 2.7;        // Meters (Assumed standard floor-to-ceiling height)

// Array of month names and average days per month
const std::string MONTH_NAMES[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};
const int MONTH_DAYS[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// Orientation solar impact factors (dimensionless modifier on cooling load)
enum Orientation { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };
const double ORIENTATION_FACTORS[4] = { 0.85, 1.10, 1.00, 1.25 };

// Struct to encapsulate building configuration parameters
struct BuildingProps {
    double floorArea;          // m^2
    double insulationRValue;   // (m^2·K)/W
    double glassArea;          // m^2
    double glassRValue;        // (m^2·K)/W
    double airChangesPerHour;  // ACH (1/h)
    int orientationIndex;      // 0: North, 1: East, 2: South, 3: West
    int occupants;             // Number of people
    double internalEquipmentW; // Internal heat load from devices (Watts)
};

// Struct to store thermal environment parameters
struct ClimateData {
    double designHeatingTemp;  // Winter outdoor design temp (°C)
    double designCoolingTemp;  // Summer outdoor design temp (°C)
    double targetHeatingSet;   // Indoor heating setpoint (°C)
    double targetCoolingSet;   // Indoor cooling setpoint (°C)
    double monthlyAvgTemp[12]; // Outdoor monthly mean temperature (°C)
};

// Struct to hold system performance factors
struct UtilityRates {
    double electricityRate;    // ₱/kWh
    double heatingCop;         // Coefficient of Performance (Heating COP)
    double coolingEer;         // Energy Efficiency Ratio / SEER converted to COP (Cooling COP)
};

// Struct for Monthly Output Results
struct MonthlyResult {
    double heatingEnergyKWh;
    double coolingEnergyKWh;
    double heatingCost;
    double coolingCost;
    double totalCost;
};

// --- Function Declarations ---
double getValidatedInput(const std::string& prompt, double minVal, double maxVal);
int getValidatedInputInt(const std::string& prompt, int minVal, int maxVal);
void inputBuildingData(BuildingProps& bldg);
void inputClimateData(ClimateData& climate);
void inputUtilityRates(UtilityRates& utility);

double calculateEnvelopeUValue(double RValue);
double calculateDesignHeatingLoad(const BuildingProps& bldg, const ClimateData& climate);
double calculateDesignCoolingLoad(const BuildingProps& bldg, const ClimateData& climate);

MonthlyResult calculateMonthlyMetrics(int monthIndex, const BuildingProps& bldg, 
                                      const ClimateData& climate, const UtilityRates& utility,
                                      double heatingSet, double coolingSet);

void displayEngineeringReport(const BuildingProps& bldg, const ClimateData& climate, 
                              const UtilityRates& utility);
void performEfficiencyComparison(const BuildingProps& bldg, const ClimateData& climate, 
                                  const UtilityRates& utility);
void runSelfTests();

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    std::cout << "=================================================================\n";
    std::cout << "      BUILDING ENERGY CONSUMPTION & COST ESTIMATOR (HVAC)        \n";
    std::cout << "=================================================================\n";

    // Run programmatic validation self-tests prior to interactive loop
    runSelfTests();

    char runMode;
    std::cout << "\nDo you want to enter custom building data? (y/n): ";
    std::cin >> runMode;

    BuildingProps bldg;
    ClimateData climate;
    UtilityRates utility;

    if (runMode == 'y' || runMode == 'Y') {
        inputBuildingData(bldg);
        inputClimateData(climate);
        inputUtilityRates(utility);
    } else {
        std::cout << "\n[INFO] Loading Default Engineering Baseline Scenario...\n";
        bldg = { 150.0, 3.5, 20.0, 0.45, 0.5, 2, 4, 300.0 };
        climate = { -10.0, 35.0, 21.0, 24.0, 
                    {-2.0, 0.0, 5.0, 11.0, 17.0, 22.0, 26.0, 25.0, 20.0, 13.0, 6.0, 0.0} };
        utility = { 11.50, 3.2, 3.5 }; // Baseline electricity rate set in PHP (₱11.50/kWh)
    }

    // Display primary engineering report
    displayEngineeringReport(bldg, climate, utility);

    // Run setpoint optimization/efficiency comparison
    performEfficiencyComparison(bldg, climate, utility);

    return 0;
}

// ============================================================================
// INPUT VALIDATION & DATA ENTRY FUNCTIONS
// ============================================================================

double getValidatedInput(const std::string& prompt, double minVal, double maxVal) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minVal && value <= maxVal) {
            return value;
        } else {
            std::cout << "  [ERROR] Invalid input. Enter a value between " 
                      << minVal << " and " << maxVal << ".\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int getValidatedInputInt(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minVal && value <= maxVal) {
            return value;
        } else {
            std::cout << "  [ERROR] Invalid input. Enter an integer between " 
                      << minVal << " and " << maxVal << ".\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void inputBuildingData(BuildingProps& bldg) {
    std::cout << "\n--- BUILDING PROPERTIES ---\n";
    bldg.floorArea = getValidatedInput("Floor Area (m^2) [10 - 10000]: ", 10.0, 10000.0);
    bldg.insulationRValue = getValidatedInput("Opaque Wall R-Value ((m^2*K)/W) [0.5 - 15.0]: ", 0.5, 15.0);
    
    // Validate glass area relative to floor area
    double maxGlass = bldg.floorArea * 0.8;
    bldg.glassArea = getValidatedInput("Window Area (m^2) [0 - " + std::to_string(maxGlass) + "]: ", 0.0, maxGlass);
    bldg.glassRValue = getValidatedInput("Window R-Value ((m^2*K)/W) [0.1 - 2.0]: ", 0.1, 2.0);
    
    bldg.airChangesPerHour = getValidatedInput("Infiltration Rate (ACH) [0.05 - 5.0]: ", 0.05, 5.0);
    
    std::cout << "Orientation (0: North, 1: East, 2: South, 3: West): ";
    bldg.orientationIndex = getValidatedInputInt("", 0, 3);
    
    bldg.occupants = getValidatedInputInt("Number of Occupants [0 - 100]: ", 0, 100);
    bldg.internalEquipmentW = getValidatedInput("Base Internal Equipment Load (Watts) [0 - 50000]: ", 0.0, 50000.0);
}

void inputClimateData(ClimateData& climate) {
    std::cout << "\n--- CLIMATE & THERMOSTAT SETTINGS ---\n";
    climate.designHeatingTemp = getValidatedInput("Winter Design Outdoor Temp (°C) [-50 - 15]: ", -50.0, 15.0);
    climate.designCoolingTemp = getValidatedInput("Summer Design Outdoor Temp (°C) [20 - 55]: ", 20.0, 55.0);
    climate.targetHeatingSet = getValidatedInput("Indoor Heating Setpoint (°C) [15 - 24]: ", 15.0, 24.0);
    climate.targetCoolingSet = getValidatedInput("Indoor Cooling Setpoint (°C) [20 - 28]: ", 20.0, 28.0);

    if (climate.targetHeatingSet >= climate.targetCoolingSet) {
        std::cout << "  [WARNING] Heating setpoint should be lower than cooling setpoint to avoid simultaneous heating/cooling. Adjusting cooling setpoint.\n";
        climate.targetCoolingSet = climate.targetHeatingSet + 2.0;
    }

    std::cout << "\nEnter Average Outdoor Temperatures for Each Month (°C):\n";
    for (int i = 0; i < 12; ++i) {
        std::string prompt = "  " + MONTH_NAMES[i] + ": ";
        climate.monthlyAvgTemp[i] = getValidatedInput(prompt, -60.0, 60.0);
    }
}

void inputUtilityRates(UtilityRates& utility) {
    std::cout << "\n--- UTILITY RATES & HVAC EFFICIENCY ---\n";
    utility.electricityRate = getValidatedInput("Electricity Cost (PHP/kWh) [0.50 - 100.0]: ", 0.50, 100.0);
    utility.heatingCop = getValidatedInput("Heating System COP [0.8 - 6.0]: ", 0.8, 6.0);
    utility.coolingEer = getValidatedInput("Cooling System Seasonal COP (EER/3.412) [1.0 - 8.0]: ", 1.0, 8.0);
}

// ============================================================================
// ENGINEERING CALCULATIONS
// ============================================================================

double calculateEnvelopeUValue(double RValue) {
    if (RValue <= 0.0) return 0.0;
    return 1.0 / RValue; // U = 1 / R [W/(m^2·K)]
}

/**
 * Calculates peak heating design load in Watts (W)
 * Formula: Q_heating = (UA_opaque + UA_glass + Q_vent) * (T_set,heat - T_outdoor,design)
 */
double calculateDesignHeatingLoad(const BuildingProps& bldg, const ClimateData& climate) {
    double wallArea = (std::sqrt(bldg.floorArea) * 4.0 * CLEAR_HEIGHT) - bldg.glassArea;
    if (wallArea < 0) wallArea = 0;

    double uWall = calculateEnvelopeUValue(bldg.insulationRValue);
    double uGlass = calculateEnvelopeUValue(bldg.glassRValue);

    double uaTotal = (wallArea * uWall) + (bldg.glassArea * uGlass) + (bldg.floorArea * uWall * 0.5); // Floor/Ceiling allowance
    
    // Infiltration loss: V_dot * rho * Cp * deltaT
    double volume = bldg.floorArea * CLEAR_HEIGHT;
    double mDot = (volume * bldg.airChangesPerHour * AIR_DENSITY) / 3600.0; // kg/s
    double cpInfiltration = mDot * AIR_SPECIFIC_HEAT; // W/K

    double deltaT = climate.targetHeatingSet - climate.designHeatingTemp;
    if (deltaT < 0) deltaT = 0;

    return (uaTotal + cpInfiltration) * deltaT; // Watts
}

/**
 * Calculates peak cooling design load in Watts (W)
 * Includes Envelope Transmission + Infiltration + Solar Orientation + Internal Occupant Gains
 */
double calculateDesignCoolingLoad(const BuildingProps& bldg, const ClimateData& climate) {
    double wallArea = (std::sqrt(bldg.floorArea) * 4.0 * CLEAR_HEIGHT) - bldg.glassArea;
    if (wallArea < 0) wallArea = 0;

    double uWall = calculateEnvelopeUValue(bldg.insulationRValue);
    double uGlass = calculateEnvelopeUValue(bldg.glassRValue);

    double uaTotal = (wallArea * uWall) + (bldg.glassArea * uGlass);
    
    double volume = bldg.floorArea * CLEAR_HEIGHT;
    double mDot = (volume * bldg.airChangesPerHour * AIR_DENSITY) / 3600.0;
    double cpInfiltration = mDot * AIR_SPECIFIC_HEAT;

    double deltaT = climate.designCoolingTemp - climate.targetCoolingSet;
    if (deltaT < 0) deltaT = 0;

    double sensibleEnvelope = (uaTotal + cpInfiltration) * deltaT;
    
    // Internal heat gains (Occupants ~ 100W sensible each + Equipment)
    double internalGain = (bldg.occupants * 100.0) + bldg.internalEquipmentW;
    
    // Solar load adjusted by building orientation factor
    double orientationFactor = ORIENTATION_FACTORS[bldg.orientationIndex];
    double solarGain = (bldg.glassArea * 250.0) * orientationFactor; // 250 W/m^2 nominal peak glass gain

    return sensibleEnvelope + internalGain + solarGain; // Watts
}

/**
 * Calculates monthly thermal energy (kWh) and utility costs in PHP.
 */
MonthlyResult calculateMonthlyMetrics(int monthIndex, const BuildingProps& bldg, 
                                      const ClimateData& climate, const UtilityRates& utility,
                                      double heatingSet, double coolingSet) {
    MonthlyResult result = { 0.0, 0.0, 0.0, 0.0, 0.0 };
    
    double avgT = climate.monthlyAvgTemp[monthIndex];
    int days = MONTH_DAYS[monthIndex];
    double hoursInMonth = days * 24.0;

    // UA Total and Infiltration calculations
    double wallArea = (std::sqrt(bldg.floorArea) * 4.0 * CLEAR_HEIGHT) - bldg.glassArea;
    if (wallArea < 0) wallArea = 0;
    double uaTotal = (wallArea * calculateEnvelopeUValue(bldg.insulationRValue)) + 
                      (bldg.glassArea * calculateEnvelopeUValue(bldg.glassRValue));
    
    double volume = bldg.floorArea * CLEAR_HEIGHT;
    double cpInfiltration = ((volume * bldg.airChangesPerHour * AIR_DENSITY) / 3600.0) * AIR_SPECIFIC_HEAT;
    double overallHeatLossCoef = uaTotal + cpInfiltration; // W/K

    double internalGainskW = ((bldg.occupants * 100.0) + bldg.internalEquipmentW) / 1000.0; // kW

    // HEATING SEASON LOGIC
    if (avgT < heatingSet) {
        double deltaT = heatingSet - avgT;
        double thermalDemandW = overallHeatLossCoef * deltaT; 
        double thermalEnergykWh = (thermalDemandW / 1000.0) * hoursInMonth;
        
        // Internal gains offset heating demand
        thermalEnergykWh -= (internalGainskW * hoursInMonth * 0.7); // 0.7 utilization factor
        if (thermalEnergykWh < 0) thermalEnergykWh = 0;

        result.heatingEnergyKWh = thermalEnergykWh / utility.heatingCop; // Electrical input energy
        result.heatingCost = result.heatingEnergyKWh * utility.electricityRate;
    }

    // COOLING SEASON LOGIC
    if (avgT > coolingSet) {
        double deltaT = avgT - coolingSet;
        double thermalDemandW = (overallHeatLossCoef * deltaT) + 
                                ((bldg.glassArea * 150.0 * ORIENTATION_FACTORS[bldg.orientationIndex]));
        
        double thermalEnergykWh = ((thermalDemandW / 1000.0) + internalGainskW) * hoursInMonth;
        
        result.coolingEnergyKWh = thermalEnergykWh / utility.coolingEer; // Electrical input energy
        result.coolingCost = result.coolingEnergyKWh * utility.electricityRate;
    }

    result.totalCost = result.heatingCost + result.coolingCost;
    return result;
}

// ============================================================================
// REPORTING & ANALYSIS
// ============================================================================

void displayEngineeringReport(const BuildingProps& bldg, const ClimateData& climate, 
                              const UtilityRates& utility) {
    double peakHeatingW = calculateDesignHeatingLoad(bldg, climate);
    double peakCoolingW = calculateDesignCoolingLoad(bldg, climate);

    std::cout << "\n=================================================================\n";
    std::cout << "                   ENGINEERING SUMMARY REPORT                    \n";
    std::cout << "=================================================================\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << " Peak Design Heating Load : " << std::setw(8) << peakHeatingW / 1000.0 << " kW (" 
              << (peakHeatingW / bldg.floorArea) << " W/m^2)\n";
    std::cout << " Peak Design Cooling Load : " << std::setw(8) << peakCoolingW / 1000.0 << " kW (" 
              << (peakCoolingW / bldg.floorArea) << " W/m^2)\n";
    std::cout << "-----------------------------------------------------------------\n";

    std::cout << "\nMONTHLY ENERGY CONSUMPTION & UTILITY COST BREAKDOWN:\n";
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << std::left << std::setw(12) << "Month" 
              << std::right << std::setw(14) << "Heat (kWh)" 
              << std::setw(14) << "Cool (kWh)" 
              << std::setw(14) << "Total (PHP)\n";
    std::cout << "-----------------------------------------------------------------\n";

    double annualHeatkWh = 0.0;
    double annualCoolkWh = 0.0;
    double annualTotalCost = 0.0;

    for (int i = 0; i < 12; ++i) {
        MonthlyResult res = calculateMonthlyMetrics(i, bldg, climate, utility, 
                                                    climate.targetHeatingSet, 
                                                    climate.targetCoolingSet);
        
        annualHeatkWh += res.heatingEnergyKWh;
        annualCoolkWh += res.coolingEnergyKWh;
        annualTotalCost += res.totalCost;

        std::cout << std::left << std::setw(12) << MONTH_NAMES[i]
                  << std::right << std::setw(14) << res.heatingEnergyKWh
                  << std::setw(14) << res.coolingEnergyKWh
                  << std::setw(14) << res.totalCost << "\n";
    }

    std::cout << "-----------------------------------------------------------------\n";
    std::cout << std::left << std::setw(12) << "ANNUAL TOTAL"
              << std::right << std::setw(14) << annualHeatkWh
              << std::setw(14) << annualCoolkWh
              << std::setw(14) << annualTotalCost << "\n";
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << " Normalized Energy Intensity : " << (annualHeatkWh + annualCoolkWh) / bldg.floorArea 
              << " kWh/m^2/year\n";
}

void performEfficiencyComparison(const BuildingProps& bldg, const ClimateData& climate, 
                                  const UtilityRates& utility) {
    std::cout << "\n=================================================================\n";
    std::cout << "         EFFICIENCY & SETPOINT OPTIMIZATION ANALYSIS             \n";
    std::cout << "=================================================================\n";

    // Standard baseline cost
    double baselineCost = 0.0;
    for (int i = 0; i < 12; ++i) {
        baselineCost += calculateMonthlyMetrics(i, bldg, climate, utility, 
                                                climate.targetHeatingSet, 
                                                climate.targetCoolingSet).totalCost;
    }

    // Eco Setpoint (-1°C Heating, +1°C Cooling)
    double ecoHeatingSet = climate.targetHeatingSet - 1.0;
    double ecoCoolingSet = climate.targetCoolingSet + 1.0;
    double ecoCost = 0.0;
    for (int i = 0; i < 12; ++i) {
        ecoCost += calculateMonthlyMetrics(i, bldg, climate, utility, 
                                            ecoHeatingSet, ecoCoolingSet).totalCost;
    }

    double savings = baselineCost - ecoCost;
    double percentSavings = (baselineCost > 0) ? (savings / baselineCost) * 100.0 : 0.0;

    std::cout << " Baseline Thermostat Settings: Heating = " << climate.targetHeatingSet 
              << "°C | Cooling = " << climate.targetCoolingSet << "°C\n";
    std::cout << " Baseline Annual Operating Cost: PHP " << baselineCost << "\n\n";

    std::cout << " Optimized Eco Settings       : Heating = " << ecoHeatingSet 
              << "°C | Cooling = " << ecoCoolingSet << "°C\n";
    std::cout << " Optimized Annual Cost         : PHP " << ecoCost << "\n";
    std::cout << " Projected Annual Savings      : PHP " << savings << " (" << percentSavings << "% reduction)\n";
    std::cout << "=================================================================\n";
}

// ============================================================================
// TESTING & VERIFICATION
// ============================================================================

void runSelfTests() {
    std::cout << "[SYSTEM VERIFICATION] Running core calculation test suite...\n";

    // Test 1: Envelope U-Value Conversion
    double testR = 2.0;
    double expectedU = 0.5;
    if (std::abs(calculateEnvelopeUValue(testR) - expectedU) < 1e-5) {
        std::cout << "  [PASS] Test 1: U-Value calculation verified.\n";
    } else {
        std::cout << "  [FAIL] Test 1: U-Value calculation out of tolerance.\n";
    }

    // Test 2: Zero Delta T Heating Load
    BuildingProps testBldg = { 100.0, 2.0, 10.0, 0.5, 0.5, 0, 0, 0.0 };
    ClimateData testClimate = { 20.0, 30.0, 20.0, 24.0, {20.0} };
    double zeroHeatLoad = calculateDesignHeatingLoad(testBldg, testClimate);
    
    if (std::abs(zeroHeatLoad) < 1e-5) {
        std::cout << "  [PASS] Test 2: Boundary case (Zero Delta-T Heating) verified.\n";
    } else {
        std::cout << "  [FAIL] Test 2: Zero Delta-T calculation failed.\n";
    }

    std::cout << "[SYSTEM VERIFICATION] All automated checks complete.\n";
}