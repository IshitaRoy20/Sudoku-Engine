# Sudoku Solver & Generator

A modular C++17 application for generating, solving, and validating Sudoku puzzles. The project implements recursive backtracking, puzzle generation with unique-solution verification, an interactive hint system, and automated testing.

## Overview

This project provides a complete Sudoku engine capable of:

* Generating valid Sudoku puzzles
* Solving Sudoku puzzles using recursive backtracking
* Supporting multiple difficulty levels
* Providing interactive hints
* Validating user-submitted solutions
* Ensuring puzzle correctness through automated tests

The codebase is organized into reusable modules with a focus on maintainability, extensibility, and clean software design.

---

## Features

### Puzzle Generation

* Generates valid 9×9 Sudoku puzzles
* Supports Easy, Medium, and Hard difficulty levels
* Produces puzzles with a unique solution
* Uses randomized board generation for puzzle variety

### Sudoku Solver

* Recursive backtracking algorithm
* Constraint-based candidate validation
* Detects unsolvable puzzle states
* Efficient state restoration during backtracking

### Interactive Gameplay

* Hint system with configurable limits
* User solution submission and verification
* Optional solution reveal
* Command-line interface for gameplay

### Validation

* Row validation
* Column validation
* 3×3 subgrid validation
* Verification that original clues remain unchanged

### Testing

* Unit tests covering:

  * Grid operations
  * Coordinate utilities
  * Puzzle solving
  * Puzzle generation
  * Solution validation

---

## Technology Stack

**Language**

* C++17

**Core Concepts**

* Recursion
* Backtracking
* Object-Oriented Programming
* Constraint Satisfaction
* Randomized Algorithms
* Unit Testing

**Standard Library Components**

* `std::array`
* `std::vector`
* `std::set`
* `std::optional`
* `std::shuffle`
* `std::mt19937`

---

## Project Structure

```text
Sudoku-Solver/
│
├── src/
│   ├── coord.h
│   ├── coord_utils.h
│   ├── grid.h
│   ├── sudoku_solver.h
│   ├── sudoku_generator.h
│   ├── sudoku_validator.h
│   └── sudoku_hint.h
│
├── tests/
│   ├── test_grid.h
│   ├── test_coord_utils.h
│   ├── test_sudoku_solver.h
│   ├── test_sudoku_generator.h
│   ├── test_sudoku_validator.h
│   └── test_sudoku_suite.cpp
│
├── main.cpp
├── sudokugenerator.cpp
├── sudokusolver.cpp
└── README.md
```

---

## Architecture

### Grid Module

Responsible for:

* Storing puzzle state
* Updating cell values
* Validating candidate placements
* Tracking pre-filled cells

### Solver Module

Implements recursive backtracking:

1. Select an empty cell
2. Generate valid candidates
3. Place a candidate
4. Recurse
5. Backtrack if necessary

### Generator Module

Creates puzzles by:

1. Generating a complete valid board
2. Removing cells according to difficulty
3. Verifying uniqueness of the resulting puzzle

### Validator Module

Ensures:

* Every row contains digits 1–9 exactly once
* Every column contains digits 1–9 exactly once
* Every 3×3 subgrid contains digits 1–9 exactly once

### Hint Module

Provides controlled assistance by revealing valid cell values from the solved board while preserving puzzle consistency.

---

## Building the Project

### Compile Generator

```bash
g++ -std=c++17 sudokugenerator.cpp -o generator
```

### Run Generator

```bash
./generator
```

---

### Compile Solver

```bash
g++ -std=c++17 sudokusolver.cpp -o solver
```

### Run Solver

```bash
./solver
```

---

### Compile Interactive Solver

```bash
g++ -std=c++17 main.cpp -o sudoku
```

### Run

```bash
./sudoku
```

---

## Running Tests

Compile:

```bash
g++ -std=c++17 tests/test_sudoku_suite.cpp -o testsuite
```

Run:

```bash
./testsuite
```

Successful execution indicates all unit tests have passed.

---

## Algorithmic Complexity

### Solving

Worst-case time complexity:

```text
O(9^(n²))
```

where `n = 9`.

Practical performance is significantly improved through constraint checking and search-space pruning.

### Space Complexity

```text
O(n²)
```

for board storage and recursive call stack usage.

---

## Engineering Challenges

Key challenges addressed during development include:

* Designing reusable Sudoku abstractions
* Managing recursive state transitions safely
* Generating randomized yet valid puzzle configurations
* Preserving unique solutions during puzzle generation
* Implementing efficient validation logic
* Building comprehensive test coverage

---

## Future Enhancements

* Graphical User Interface (GUI)
* Web-based deployment
* Puzzle save/load support
* Timer and scoring system
* Advanced solving strategies
* Difficulty estimation based on solving techniques
* Additional Sudoku variants
* Performance benchmarking and analytics

---

## Learning Outcomes

This project strengthened practical experience in:

* Recursive algorithm design
* Backtracking optimization
* Software modularization
* Test-driven development
* Data structure selection
* Constraint satisfaction problems

---

## Author

Developed as a systems-oriented C++ project to explore algorithm design, puzzle generation, and software engineering best practices.
