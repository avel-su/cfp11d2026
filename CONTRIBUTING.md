# Contributing to the CFP11 Class Project

This repository is being used for a classroom exercise.

The goal is to learn a simple development workflow:

**change → test → commit → push → Pull Request**

You are not expected to already know GitHub. Learning the workflow is part of the exercise.

## Before you start

You should have:

- a GitHub account
- Git installed, or a GitHub-supported way to clone the repository
- a C++ compiler
- a C++ development environment

## Basic workflow

### 1. Fork the repository

Create your own copy of the classroom repository on GitHub.

### 2. Clone your fork

Download your fork to your computer.

### 3. Create a branch

Create a separate branch for your task.

Example:

```text
maria-even-odd
```

### 4. Make your change

Open:

```text
src/main.cpp
```

Complete one task from `TASKS.md`.

### 5. Compile and test

Make sure the program compiles.

Run it with different inputs.

Think about whether the output makes sense.

### 6. Commit

Create a commit that describes your change.

Good example:

```text
Add even or odd check
```

Poor example:

```text
stuff
```

### 7. Push

Push your branch to your GitHub fork.

### 8. Open a Pull Request

Create a Pull Request from your branch to the original classroom repository.

In the Pull Request description, write:

```text
What I changed:
...

How I tested it:
...

What I learned:
...
```

## Pull Request checklist

Before submitting:

* [ ] My program compiles.
* [ ] I tested my change.
* [ ] I changed only what was necessary.
* [ ] I can explain the code I added.
* [ ] My commit message describes the change.
* [ ] My Pull Request explains what I changed.
* [ ] My Pull Request explains how I tested it.

## Keep the code simple

This is an introductory programming exercise.

Prefer code that another first-year student can easily read.

Do not add:

* external libraries
* frameworks
* classes
* complex data structures
* advanced C++ features
* unnecessary files

unless the instructor specifically asks for them.

A small working change is better than a complicated unfinished change.
