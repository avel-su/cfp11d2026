# P3 – Pipe Flow & Pressure Drop Calculator

## What it does
Interactive C++ program that, for a circular pipe and a Newtonian fluid, computes velocity/discharge,
Reynolds number, flow regime, Darcy friction factor, pressure drop (Darcy-Weisbach) and head loss,
and prints a formatted report in engineering units.

## Key calculations (SI)
- Re = rho V D / mu; laminar < 2300, transitional 2300-4000, turbulent > 4000
- Laminar: f = 64/Re. Turbulent: Haaland, 1/sqrt(f) = -1.8 log10[(e/D/3.7)^1.11 + 6.9/Re]
- Colebrook-White solved by fixed-point iteration as an independent cross-check
- dP = f (L/D) (rho V^2 / 2); h = dP / (rho g)

## Structure
- `calculations.h/.cpp` – pure calculation + validation functions (no I/O)
- `main.cpp` – input prompts, menus, report formatting
- `tests.cpp` – 34 automated checks

## Testing
Hand calculation (water, D = 0.1 m, V = 2 m/s, L = 100 m, steel): f = 0.01838, dP = 36.7 kPa.
Laminar case checked against Hagen-Poiseuille (6400 Pa). Haaland vs Colebrook within 2.5% over 25 cases.
Boundary values (min/max D, L, V, e = 0) and invalid inputs (zero/negative/NaN/non-numeric) tested.

## Assumptions / limitations
Steady, incompressible, fully developed flow; straight horizontal full pipe; constant properties;
no minor losses; Haaland valid for e/D <= 0.05; transitional-regime results are estimates (program warns).

## Build
    g++ -std=c++17 -Wall -Wextra -o pipeflow main.cpp calculations.cpp
    g++ -std=c++17 -Wall -Wextra -o tests tests.cpp calculations.cpp
