# Contributing to CFP11 2026 — C++ Engineering Project

This repository is used for the semester-end **C++ engineering project** for CFP11: Computer Fundamentals and Programming.

The goal is to develop a complete engineering program in C++ and use proper software-development workflow with GitHub.

**workflow: plan → code → test → commit → push → Pull Request**

Learning the GitHub workflow is part of the exercise.

## Before you start coding

You should have:

- a GitHub account
- Git installed on your computer
- a C++ compiler (g++, clang, or Visual C++)
- a code editor or IDE

### 1. Learn GitHub basics

Read the GitHub Learning guide:

- https://learn.github.com/

### 2. Understand your project

Read [PROJECTS.md](PROJECTS.md).

Choose one of the P1–P10 engineering projects, or develop a qualifying P11 project.

No instructor approval is required for a proposed project.

Understand:

- The engineering problem
- Required inputs and outputs
- Key calculations and formulas
- Engineering assumptions and constraints
- What testing means for this project

### 3. Plan your program structure

Before coding, sketch out:

- What data will you need? (consider structs or arrays)
- What functions will you need?
- What does the main flow look like?
- Where do you need validation?
- How will you organize the program?

## Development workflow

### 1. Fork the repository

Click the **Fork** button to create your own copy on GitHub.

### 2. Clone your fork

Download your fork to your computer:

```bash
git clone https://github.com/YOUR_USERNAME/cfp11d2026.git
cd cfp11d2026
```

### 3. Create a branch

Create a branch for your project work:

```bash
git checkout -b my-project-name
```

Example branch name: `beam-calculator` or `circuit-analyzer`

### 4. Develop your program

- Open `src/main.cpp`.
- Write your C++ program to solve the engineering problem.
- You may substantially modify or replace the starter code.
- Compile regularly to catch errors early.
- Test with realistic inputs and boundary cases.

### 5. Compile and test

Compile your program:

```bash
g++ -o myprogram src/main.cpp
```

(Adjust the compiler and flags based on your setup.)

Test with:

- Normal engineering scenarios
- Boundary conditions (min/max valid values)
- Invalid inputs (negative when positive expected, zero when non-zero required, etc.)
- Verify calculations against hand calculations or reference values

### 6. Commit meaningful progress

Commit regularly, not just once at the end:

```bash
git add src/main.cpp
git commit -m "Add input validation for beam properties"
```

Good commit messages:

- "Add structural analysis calculation"
- "Implement file I/O for saving results"
- "Add input validation"
- "Test circuit analysis with realistic values"

Poor commit messages:

- "stuff"
- "fixed"
- "update"
- "final"

### 7. Push your branch

Push your work to GitHub:

```bash
git push origin my-project-name
```

### 8. Open a Pull Request

Go to your fork on GitHub.

Click "New Pull Request" and select your branch.

In the Pull Request description, explain:

**Project:** [Name of the engineering project]

**What I developed:**
[Brief description of what the program does]

**Engineering approach:**
[Key calculations, formulas, assumptions, design decisions]

**How I tested it:**
[Test cases used, including realistic cases and boundary conditions]

**Program structure:**
[Brief overview of functions and organization]

**Known limitations:**
[Any edge cases or constraints]

## Engineering quality expectations

### Code organization

- Write functions for logical blocks of code
- Avoid putting the entire program into `main()`
- Use meaningful function names (e.g., `calculateBeamStress()`, `validateInput()`)
- Keep functions focused on single tasks

### Documentation

Include comments explaining:

- What each function does
- Complex calculations or formulas
- Engineering assumptions
- Unit conventions (meters, kN, MPa, etc.)
- Validation criteria

Example:

```cpp
// Calculate bending stress using flexure formula: sigma = M * c / I
// where M = bending moment, c = distance to neutral axis, I = second moment
double stress = bendingMoment * distanceToNA / momentOfInertia;
```

### Variable names

Use clear, descriptive names:

```cpp
// Good
double beamLength;
double materialYieldStrength;
double safetyFactor;

// Avoid
double l;
double ys;
double sf;
```

### Input validation

Validate all user input:

```cpp
if (beamLength <= 0) {
    cerr << "Error: Beam length must be positive." << endl;
    return 1;
}
```

### Error handling

Handle errors gracefully:

- Check file I/O success
- Provide meaningful error messages
- Use sensible defaults when appropriate
- Avoid crashing on invalid input

### Testing

Test your program with:

- **Normal cases**: typical engineering values
- **Boundary cases**: minimum and maximum valid values
- **Invalid input**: negative when positive expected, zero when non-zero required, etc.
- **Reference values**: verify calculations against hand calculations or known results

Document your testing in the Pull Request description.

### File I/O

If your project requires file I/O:

- Save and load data in a human-readable format
- Include appropriate file error handling
- Ensure results can be verified later

## Pull Request checklist

Before submitting your Pull Request:

- [ ] Program compiles without errors
- [ ] Program runs and produces correct output
- [ ] Realistic test cases have been performed
- [ ] Boundary and invalid cases have been tested
- [ ] Important calculations have been verified
- [ ] Functions are used appropriately
- [ ] Code is readable and well-organized
- [ ] Engineering formulas and assumptions are documented
- [ ] Input validation is implemented
- [ ] Error messages are clear and helpful
- [ ] Meaningful commit history is present
- [ ] Pull Request explains the project and testing

## Keep the code focused

This is an engineering project, not a software product.

Focus on:

- Solving the engineering problem correctly
- Using appropriate C++ concepts
- Clear, readable code
- Proper testing and validation

Do not add:

- external libraries (unless instructor approves)
- graphics or complex visualization
- databases
- web frameworks
- unnecessary complexity

A well-structured, thoroughly-tested engineering program is better than an elaborate but incomplete system.

## Questions or issues?

- Review the project description in [PROJECTS.md](PROJECTS.md)
- Check [SUBMISSION.md](SUBMISSION.md) for submission requirements
- Ask your instructor for help

Good luck with your engineering project!
