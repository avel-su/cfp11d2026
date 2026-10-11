# Pipe Flow & Pressure Drop Calculator

## Project Overview

The Pipe Flow & Pressure Drop Calculator is a C++ console application that estimates fluid velocity, Reynolds number, friction factor, pressure drop, and head loss in a straight circular pipe.

The program is designed to help engineering students understand basic fluid mechanics calculations.

## Objectives

* Calculate the cross-sectional area of a circular pipe.
* Determine average fluid velocity from volumetric flow rate.
* Calculate the Reynolds number and classify the flow regime.
* Estimate the Darcy friction factor.
* Calculate pressure drop and head loss due to pipe-wall friction.
* Apply input validation and modular programming in C++.

## Input Parameters

| Parameter               | Symbol | Unit  |
| ----------------------- | ------ | ----- |
| Volumetric flow rate    | Q      | m³/s  |
| Pipe inside diameter    | D      | m     |
| Pipe length             | L      | m     |
| Fluid density           | ρ      | kg/m³ |
| Dynamic viscosity       | μ      | Pa·s  |
| Pipe absolute roughness | ε      | m     |

All input values must use SI units. Flow rate, diameter, length, density, and viscosity must be greater than zero. Roughness must be zero or greater.

## Engineering Formulas

### 1. Cross-sectional area

A = πD² / 4

### 2. Average fluid velocity

v = Q / A

### 3. Reynolds number

Re = ρvD / μ

### 4. Darcy friction factor

For laminar flow:

f = 64 / Re

For turbulent flow, the program uses the Haaland approximation:

1 / √f = -1.8 log₁₀[(ε / 3.7D)¹·¹¹ + 6.9 / Re]

The transitional flow range is treated as uncertain, and its friction factor is an estimate.

### 5. Pressure drop

ΔP = f(L / D)(ρv² / 2)

### 6. Head loss

h_f = ΔP / (ρg)

where g = 9.81 m/s².

## Flow Classification

* Laminar: Re < 2300
* Transitional: 2300 ≤ Re ≤ 4000
* Turbulent: Re > 4000

## Assumptions and Limitations

* The pipe is straight and circular.
* Flow is steady, and the fluid properties are constant.
* The pipe diameter and roughness are uniform.
* The Darcy-Weisbach equation is used for frictional pressure loss.
* The Haaland equation approximates the friction factor for turbulent flow.
* Transitional flow calculations are approximate.
* Losses from bends, valves, fittings, entrances, and exits are not included.
* The calculation does not include elevation changes or pump effects.

## How to Run

1. Open PipeFlowCalculator.cbp in Code::Blocks.
2. Build and run the project using F9.
3. Choose option 1 to calculate pipe flow.
4. Enter all required values in the specified SI units.
5. Review the displayed results.
6. Choose option 2 to exit.

## Sample Test Inputs

| Parameter            | Test value |
| -------------------- | ---------: |
| Volumetric flow rate | 0.001 m³/s |
| Pipe inside diameter |     0.05 m |
| Pipe length          |       20 m |
| Fluid density        |  998 kg/m³ |
| Dynamic viscosity    | 0.001 Pa·s |
| Pipe roughness       | 0.000045 m |

## Expected Sample Results

| Output                    | Approximate value |
| ------------------------- | ----------------: |
| Pipe cross-sectional area |       0.001963 m² |
| Average fluid velocity    |      0.509296 m/s |
| Reynolds number           |      25413.861313 |
| Flow regime               |         Turbulent |
| Darcy friction factor     |          0.026187 |
| Pressure drop             |    1355.760712 Pa |
| Head loss                 |        0.138479 m |

## Input Validation

The program rejects non-numeric input at measurement prompts, requires positive flow rate, diameter, length, density, and viscosity, and allows zero roughness but rejects negative roughness.

## Technologies Used

* C++
* Code::Blocks IDE
* GNU GCC Compiler
