# Project Topics for CFP11d2026

Choose **one** engineering project for your final C++ submission.

Each project is designed to solve a real engineering problem and should integrate multiple C++ concepts you've learned:

- functions and modularity
- data structures (arrays, structs)
- file I/O
- input validation and error handling
- algorithm design

You may also propose your own engineering problem with instructor approval.

---

## P1 — Beam Load Calculator

**Description:**

Build a program to calculate stresses and deflection in a simply supported beam.

**Requirements:**

- Input beam properties (length, material, cross-section)
- Input load configuration (point loads, distributed loads)
- Calculate reaction forces
- Calculate maximum bending stress and shear stress
- Calculate deflection at midpoint
- Validate that stress is within material limits
- Generate a report with results
- Save calculations to file for later review

**Concepts:**

- structs for beam and load data
- formulas for structural analysis
- file I/O
- input validation
- engineering calculations

---

## P2 — Electrical Circuit Analyzer

**Description:**

Create a program to analyze DC circuits with resistors.

**Requirements:**

- Input circuit topology (series/parallel)
- Input resistance values and voltage source
- Calculate total resistance
- Calculate current through each branch
- Calculate voltage drops and power dissipation
- Check if resistor power ratings are exceeded
- Generate circuit analysis report
- Save circuit data for later use

**Concepts:**

- data structures for circuit components
- recursive or iterative calculations
- file I/O
- validation of electrical properties
- engineering formulas

---

## P3 — Pipe Flow & Pressure Drop Calculator

**Description:**

Build a fluid mechanics calculator for pipe flow analysis.

**Requirements:**

- Input pipe properties (diameter, length, material roughness)
- Input fluid properties (viscosity, density)
- Input flow rate or velocity
- Calculate Reynolds number (laminar/turbulent)
- Calculate friction factor (Haaland or Moody)
- Calculate pressure drop using Darcy-Weisbach equation
- Calculate flow velocity and discharge
- Report if flow is laminar or turbulent
- Save calculations to file

**Concepts:**

- structs for pipe and fluid data
- conditional logic for flow regimes
- engineering formulas and iteration
- file I/O
- input validation

---

## P4 — HVAC System Energy Estimator

**Description:**

Create a program to estimate energy consumption and costs for heating/cooling.

**Requirements:**

- Input building properties (area, insulation R-value, orientation)
- Input climate data (outdoor temperature, design conditions)
- Input occupancy and usage patterns
- Calculate heating and cooling loads
- Estimate annual energy consumption
- Calculate monthly and annual utility costs
- Generate energy report
- Compare efficiency with different system settings
- Save estimates to file

**Concepts:**

- structs for building and climate data
- heat transfer calculations
- file I/O
- conditional logic for seasons
- cost and efficiency analysis

---

## P5 — Concrete Mix Design Calculator

**Description:**

Build a tool for designing concrete mixes by weight and volume proportions.

**Requirements:**

- Input desired compressive strength
- Input cement, sand, aggregate, water ratios
- Calculate batch quantities for a given volume
- Validate mix proportions (e.g., water-cement ratio)
- Calculate expected compressive strength (empirical formula)
- Calculate material costs
- Adjust mix to meet strength or cost requirements
- Generate mix design report
- Save design to file for reference on site

**Concepts:**

- structs for concrete properties
- engineering formulas
- optimization and adjustment logic
- file I/O
- validation of material properties

---

## P6 — Traffic Signal Timing Optimizer

**Description:**

Create a program to optimize traffic signal timing for an intersection.

**Requirements:**

- Input vehicle arrival rates per lane
- Input desired maximum wait time
- Calculate cycle length
- Distribute green time among phases
- Calculate average delay and queue length
- Generate timing report
- Compare different signal plans
- Save recommended timings to file
- Account for peak and off-peak periods

**Concepts:**

- data structures for traffic flow
- queuing theory calculations
- optimization logic
- conditional analysis
- file I/O and reporting

---

## P7 — Structural Steel Member Checker

**Description:**

Build a program to verify if a steel member meets design requirements.

**Requirements:**

- Input member properties (section, length, grade)
- Input applied loads (axial, bending, shear)
- Calculate stresses (axial, bending, shear)
- Check against allowable stresses (AISC or local code)
- Calculate capacity utilization ratio
- Assess buckling (slenderness ratio)
- Generate pass/fail report
- Suggest larger section if needed
- Save member check calculations

**Concepts:**

- structs for member and load data
- conditional logic for pass/fail
- engineering design formulas
- file I/O
- compliance checking

---

## P8 — Water Tank Design & Analysis

**Description:**

Create a program for designing and analyzing water storage tanks.

**Requirements:**

- Input tank dimensions and material
- Calculate volume and mass
- Calculate hydrostatic pressure at depth
- Calculate wall stress from internal pressure
- Estimate material and construction cost
- Calculate minimum wall thickness needed
- Design overflow and drain systems
- Generate tank design report
- Save design parameters to file

**Concepts:**

- structs for tank properties
- geometric and hydrostatic calculations
- material strength formulas
- file I/O
- design validation

---

## P9 — Motor Selection & Efficiency Calculator

**Description:**

Build a tool to select appropriately-sized motors and calculate efficiency.

**Requirements:**

- Input required power and torque
- Input operating conditions (speed, load factor)
- Calculate required motor power rating
- Check against available motor sizes
- Calculate efficiency at partial loads
- Estimate annual energy cost
- Compare cost of motors at different efficiencies
- Generate motor selection report
- Save selection justification to file

**Concepts:**

- arrays or structs for motor catalog data
- conditional logic for motor selection
- efficiency and cost calculations
- file I/O
- optimization

---

## P10 — Battery State-of-Charge Estimator

**Description:**

Create a program to estimate battery state-of-charge and lifespan.

**Requirements:**

- Input battery specifications (capacity, voltage, chemistry)
- Input charge/discharge cycle data
- Calculate current state-of-charge
- Estimate battery lifespan based on cycles and depth
- Track charging/discharging history
- Alert when battery needs replacement
- Calculate replacement cost
- Generate battery health report
- Save history to file

**Concepts:**

- structs for battery data
- charge calculation algorithms
- lifespan estimation formulas
- file I/O for history tracking
- conditional alerts and warnings

---

## P11 — Propose Your Own Project

**Option:** If you have an engineering problem or calculation tool in mind, you may propose your own project.

**Requirements for approval:**

- The project should solve a real engineering problem
- It should require at least 3-4 functions
- It should involve file I/O (save/load data)
- It should include input validation
- It should be completable in one semester
- Get your instructor's written approval before starting

**Examples of student-proposed projects:**

- Solar panel angle and output calculator
- Road grade and sight distance checker
- Irrigation system cost analyzer
- Wind turbine power estimator
- Boiler efficiency and fuel cost calculator
- Bridge load rating calculator

Submit your project proposal to your instructor with:

- Problem description
- Required inputs and outputs
- Key calculations or algorithms
- Why it's relevant to your engineering field

---

## Project Submission Guidelines

### 1. Code organization

- Write modular code using functions
- Avoid putting everything in `main()`
- Use meaningful function and variable names
- Keep functions focused on single tasks

### 2. Engineering documentation

- Include comments explaining formulas and calculations
- Document assumptions and limitations
- Reference any standards or codes used (e.g., AISC, ACI)
- Explain validation criteria

### 3. Testing

- Test with realistic engineering scenarios
- Verify calculations against hand calculations or reference values
- Test boundary conditions (min/max values)
- Test invalid inputs

### 4. User interface

- Provide clear prompts for input
- Display results in engineering units
- Include units in output (meters, kN, MPa, etc.)
- Generate formatted reports

### 5. Data persistence

- Use file I/O to save and load project data
- Make data human-readable or properly formatted
- Ensure calculations can be verified later

### 6. Error handling

- Validate all user input
- Check for physically impossible values
- Provide meaningful error messages
- Handle file I/O errors gracefully

---

## Submission checklist

- [ ] Code compiles without errors
- [ ] Program runs and produces correct output
- [ ] Calculations verified against reference values
- [ ] Code is organized into functions
- [ ] File I/O is working correctly
- [ ] Pull request includes clear description
- [ ] Documentation explains engineering problem and approach
- [ ] Instructions for compiling and running are provided
- [ ] I can explain my design choices and calculations

---

## Need help choosing?

- Pick a project from your major field (civil, mechanical, electrical)
- Start with a simpler project if you're less confident
- More complex projects let you showcase deeper engineering knowledge
- Ask your instructor if you're unsure or want to propose your own

Good luck with your final project!
