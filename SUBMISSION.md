# Final Submission Requirements

## Your project

You have developed a complete C++ engineering project based on one of the topics in [PROJECTS.md](PROJECTS.md).

The project is your final submission for CFP11: Computer Fundamentals and Programming.

## Repository structure

Your submission should follow this structure:

```
YourLastName/
├── main.cpp
├── calculations.cpp (if needed)
├── calculations.h (if needed)
└── demo.mp4
```

All your source code files and demo video should be in a folder named with your **last name**.

## GitHub submission

Your completed project must be submitted through GitHub:

### Repository contents

Your fork should contain:

- **YourLastName/** folder with all .cpp and .h files
- **YourLastName/demo.mp4** (or video file with your chosen format)
- **Meaningful commit history** showing development progress

### Pull Request

Submit a Pull Request from your branch to the original repository.

The Pull Request description must explain:

1. **Project name** — which P1–P11 project you chose
2. **Engineering problem** — what real-world problem the program solves
3. **Program functionality** — what the program does
4. **Engineering calculations** — important formulas, concepts, or calculations used
5. **Design decisions** — why you organized the code the way you did
6. **Testing** — how you tested the program (test cases, verification methods)
7. **Known limitations** — any edge cases or constraints
8. **File location** — all files are in the `YourLastName/` folder

## Testing requirements

Before submitting, verify that:

- **The program compiles** without errors or warnings
- **Normal cases work** — test with typical engineering values
- **Boundary cases work** — test with minimum and maximum valid values
- **Invalid inputs are handled** — the program should reject or handle invalid data appropriately
- **Calculations are verified** — verify key calculations against hand calculations or reference values (where applicable)

Document your testing in the Pull Request description.

Example:

```
Test case 1 (normal): Beam length = 10m, load = 50kN
  Expected: Bending stress = 125 MPa
  Actual: 125 MPa ✓

Test case 2 (boundary): Beam length = 0.1m
  Expected: Program rejects or handles edge case
  Result: Program rejects with appropriate message ✓

Test case 3 (invalid): Beam length = -5m
  Expected: Error message
  Result: "Error: Length must be positive" ✓
```

## Demo video

Create a **maximum 3-minute video** demonstrating your project.

**Save the video in your student folder** with your submission (e.g., `Smith/demo.mp4`).

The video should:

1. **Show the engineering problem** — briefly explain what problem you are solving
2. **Run the program** — show the program compiling and executing
3. **Demonstrate important features** — show key inputs and outputs
4. **Show important code** — briefly display key functions or calculations
5. **Explain testing** — show how you verified calculations or tested edge cases
6. **Reflect on learning** — mention something you learned or found challenging

Keep the video short, clear, and focused.

## Code quality

Your submission should demonstrate:

- **Readability** — code that another engineering student can understand
- **Modularity** — appropriate use of functions to organize logic
- **Validation** — input validation and error handling
- **Engineering correctness** — calculations that are correct and verifiable
- **Documentation** — comments explaining formulas and assumptions

## Final checklist

Before submitting your Pull Request, confirm:

### Project completion

- [ ] Project is complete and working
- [ ] All major features are implemented
- [ ] Program achieves the goals stated in [PROJECTS.md](PROJECTS.md)

### Code quality

- [ ] Program compiles without errors
- [ ] Code is organized into functions
- [ ] Code is readable and understandable
- [ ] Variable and function names are meaningful
- [ ] Engineering formulas and assumptions are documented
- [ ] Input is validated
- [ ] Errors are handled gracefully

### Testing

- [ ] Realistic test cases were used
- [ ] Results were verified (hand calculation, reference values, etc.)
- [ ] Boundary conditions were tested
- [ ] Invalid inputs were tested
- [ ] Program behaves correctly in all test cases

### GitHub submission

- [ ] Fork of the repository is created
- [ ] Branch is created for the project
- [ ] Student folder created with last name
- [ ] All source code (.cpp, .h files) in student folder
- [ ] Demo video in student folder
- [ ] Code is committed with meaningful messages
- [ ] Branch is pushed to GitHub
- [ ] Pull Request is opened
- [ ] Pull Request describes the project clearly
- [ ] Pull Request explains testing approach
- [ ] Pull Request mentions file location

## Important notes

- This is individual work — do not copy code from other students
- Ask your instructor if you are stuck
- Late submissions will be handled per the course syllabus
- Your instructor may ask you to explain your code during office hours or after submission

## Getting help

- Review the project description in [PROJECTS.md](PROJECTS.md)
- Check [CONTRIBUTING.md](CONTRIBUTING.md) for the GitHub workflow
- Ask your instructor for clarification
- Discuss concepts with classmates, but write your own code

Good luck with your final submission!
