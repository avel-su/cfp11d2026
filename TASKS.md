# Student Tasks

Choose **one** task unless your instructor assigns you a specific task.

The tasks are intentionally small.

The purpose is to practice changing an existing program, testing your change, and explaining what you did.

---

## T1 — Ask for a name

Change the program so that it asks the user for their name and displays a greeting.

Example:

```text
Enter your name: Ana
Hello, Ana!
```

Concepts:

* string input
* variables
* output

---

## T2 — Add two numbers

Change the program so that it asks for two numbers and displays their sum.

Example:

```text
Enter the first number: 8
Enter the second number: 5
Sum: 13
```

Concepts:

* variables
* input
* arithmetic
* output

---

## T3 — Positive, negative, or zero

Ask the user for a number and report whether it is:

* positive
* negative
* zero

Example:

```text
Enter a number: -4
The number is negative.
```

Concepts:

* input
* `if`
* `else if`
* `else`

---

## T4 — Even or odd

Ask the user for an integer and report whether it is even or odd.

Example:

```text
Enter an integer: 17
The number is odd.
```

Concepts:

* input
* conditions
* remainder operator `%`

---

## T5 — Count from 1

Ask the user for a positive integer and display the numbers from 1 up to that number.

Example:

```text
Enter a number: 5

1
2
3
4
5
```

Concepts:

* `for` loop
* loop counter
* output

---

## T6 — Sum from 1

Ask the user for a positive integer and calculate the sum of all integers from 1 up to that number.

Example:

```text
Enter a number: 5
Sum: 15
```

Concepts:

* accumulator
* `for` loop
* arithmetic

---

## T7 — Multiplication table

Ask the user for a number and display its first 10 multiples.

Example:

```text
Enter a number: 7

7
14
21
28
35
42
49
56
63
70
```

Concepts:

* `for` loop
* multiplication
* loop counter

---

## T8 — Create a function

Move one calculation currently performed in `main()` into a separate function.

The function should receive the necessary value as a parameter and return the result.

Keep the function simple enough that you can explain:

* its parameter
* its return value
* where it is called

Concepts:

* function
* parameter
* return value
* function call

---

## T9 — Create an even-number function

Create a function that receives an integer and returns whether the number is even.

For example, a function could behave conceptually like:

```text
isEven(8) → true
isEven(7) → false
```

Then use the function in `main()`.

Concepts:

* functions
* parameters
* return values
* conditions
* `%`

---

## T10 — Validate input

Choose an input that should be positive.

Change the program so that it does not continue normally when the user enters a negative number.

For example:

```text
Enter a positive number: -3
Please enter a positive number.
```

You may use a loop if appropriate.

Concepts:

* conditions
* input validation
* loops

---

## T11 — Improve the user interaction

Improve the prompts and output of the existing program so that another student can understand what the program is doing without seeing the source code.

Do not change the basic purpose of the program.

Focus on:

* clear prompts
* readable output
* sensible wording

Concepts:

* input/output
* program usability
* clear communication

---

## T12 — Add a test example

Add a short comment to the program showing:

1. an example input
2. the expected output

For example:

```cpp
// Example:
// Input: 5
// Expected output: 15
```

Choose a meaningful example related to what the program currently does.

Concepts:

* comments
* testing
* expected results

---

# Rules

* Choose one task.
* Keep your change small.
* Do not rewrite the entire program.
* Do not copy another student's solution.
* Test your program before submitting it.
* Make sure the program still compiles.
* Be able to explain the code you added.

If you are unsure how to solve a task, ask your instructor rather than copying a solution.
