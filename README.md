# 🧠 Sudoku Engine — A Complete Sudoku Ecosystem Built in Modern C++17

> Generate puzzles. Verify uniqueness. Play interactively. Get hints. Undo moves. Solve instantly.

Built with modular architecture, MRV-optimized backtracking, automated tests, and a polished terminal experience.

A feature-rich terminal-based Sudoku game and solver written in **C++17**.

Play a generated puzzle, challenge yourself across multiple difficulty levels, use hints and pencil marks, or paste any Sudoku puzzle from the internet and let the engine solve it instantly.

---

## ✨ Features

### 🎮 Interactive Play Mode
- Generate fresh Sudoku puzzles instantly
- Three difficulty levels: Easy, Medium, Hard
- Guaranteed **unique-solution puzzles**
- Responsive menu-driven CLI experience

### 🧠 Intelligent Solver
- Solves any valid Sudoku puzzle
- Uses **Backtracking + MRV (Minimum Remaining Values) heuristic**
- Efficient search ordering for faster solving

### 💡 Player Assistance
- Up to **3 smart hints** per game
- Pencil-mark candidate system
- Undo support with move history tracking
- Instant board validation

### 🛡️ Robust Input Handling
- Full validation for every command and move
- Prevents modification of original clue cells
- Safe row-by-row puzzle entry in solve mode
- Clear re-prompts for invalid input

### 📊 Gameplay Tracking
- Live timer
- Mistake counter (5 lives)
- Hint tracking
- Undo stack

### 🎨 Rich Terminal UI
- Colour-coded board rendering
- Read-only clues displayed distinctly
- Correct placements highlighted
- Incorrect placements highlighted
- Clean and intuitive CLI interface

### 🧪 Tested Components
- Grid tests
- Solver tests
- Generator tests
- Validator tests
- Utility tests
- Performance tests

---

## 📋 Prerequisites

| Tool | Minimum Version |
|--------|----------------|
| GCC / Clang / MSVC | C++17 support |
| CMake (recommended) | 3.15+ |

---

## 🚀 Build & Run
```
cd build
cmake ..
make
./sudoku
```

---

## 🎮 Usage

### Main Menu

```text
┌──────── MAIN MENU ────────────────────┐
│  1. Play  (generate a new puzzle)     │
│  2. Solve (enter your own puzzle)     │
│  3. Quit                              │
└───────────────────────────────────────┘
Choice [1-3]:
```

---

### 🎲 Play Mode

Choose a difficulty level and start playing.

#### Available Commands

| Key | Action |
|------|---------|
| `p` | Place a number |
| `e` | Erase a number |
| `n` | Toggle pencil marks |
| `h` | Get a hint |
| `u` | Undo last move |
| `c` | Check board |
| `s` | Show solution |
| `q` | Quit game |

### Status Bar

```text
───────────────────────────────────────────
  ⏱  03:14   │   ❤  4/5   │   💡  2/3 hints
───────────────────────────────────────────
```

---

### 🧩 Solve Mode

Enter any Sudoku puzzle row by row/ or 81 numbers together using:

- Numbers `1-9`
- `0` for empty cells

Example:

```text
Row 1/9 : 5 3 0 0 7 0 0 0 0
Row 2/9 : 6 0 0 1 9 5 0 0 0
Row 3/9 : 0 9 8 0 0 0 0 6 0
...
```

The engine validates every row before proceeding.

Once all rows are entered, the completed solution is displayed instantly.

---

## 🎨 Board Colours

| Colour | Meaning |
|----------|----------|
| **Bold White** | Original clue |
| **Cyan** | Correct player entry |
| **Red** | Incorrect player entry |
| **Dim ·** | Pencil-mark candidates |

---

## 📂 Project Structure

```text
Sudoku-Engine/
├── CMakeLists.txt
├── README.md
├── main.cpp
├── sudokugenerator.cpp
│
├── src/
│   ├── coord.h
│   ├── coord_utils.h
│   ├── game_engine.h
│   ├── grid.h
│   ├── sudoku_generator.h
│   ├── sudoku_hint.h
│   ├── sudoku_solver.h
│   └── sudoku_validator.h
│
├── tests/
   ├── test_coord_utils.h
   ├── test_grid.h
   ├── test_performance.h
   ├── test_sudoku_generator.h
   ├── test_sudoku_solver.h
   ├── test_sudoku_validator.h
   └── test_sudoku_suite.cpp
```

---

## 🏗️ Architecture

```text
main.cpp
│
▼
GameEngine
├── Puzzle Generation
│   └── SudokuGenerator
│
├── Gameplay Layer
│   ├── Move Validation
│   ├── Undo Stack
│   ├── Hint System
│   ├── Pencil Marks
│   └── Timer Tracking
│
├── Solve Mode
│   └── SudokuSolver
│
└── Grid Management
    └── Grid
```

### 🎯 Design Highlights

- Single Responsibility Principle across modules
- No global variables
- Immutable clue preservation
- Stack-based O(1) undo system
- Modular architecture
- Efficient MRV-driven solving
- Testable components

---

## ⚙️ Algorithms

### 🎲 Puzzle Generator

The generator creates high-quality playable Sudoku puzzles:

1. Start with an empty board
2. Generate a complete valid solution using:
   - Backtracking
   - MRV heuristic
3. Randomize candidate order for puzzle variety
4. Remove cells one by one
5. After every removal:
   - Verify the puzzle still has exactly one solution
6. Continue until target difficulty is reached

| Difficulty | Cells Removed | Cells Visible |
|------------|--------------|---------------|
| Easy | 35 | 46 |
| Medium | 45 | 36 |
| Hard | 55 | 26 |

---

### 🧠 Sudoku Solver

Uses:

- Recursive Backtracking
- Minimum Remaining Values (MRV)

MRV always selects the cell with the fewest legal candidates first, dramatically reducing the search space compared to naïve row-major solving.

---

### 🔒 Uniqueness Verification

To ensure generated puzzles are valid:

1. Temporarily remove a cell
2. Run a solution-counting solver
3. Stop searching after 2 solutions
4. Accept removal only if:

```text
solutions == 1
```

This guarantees every generated puzzle has exactly one valid solution.

---

## 🚀 Engineering Highlights

This project goes beyond a simple Sudoku solver.

### What Makes It Interesting?

✅ Unique-solution puzzle generation

✅ MRV-optimized solver

✅ Modular engine architecture

✅ Undo system with move history

✅ Pencil-mark candidate support

✅ Hint generation system

✅ Full interactive CLI gameplay

✅ Robust input validation

✅ Automated testing suite

✅ Separation of concerns

Unlike many Sudoku projects that only solve puzzles, this engine supports the entire Sudoku lifecycle:

```text
Generate
    ↓
Validate
    ↓
Play
    ↓
Hint
    ↓
Undo
    ↓
Solve
```

---

## 🧪 Testing

The project includes dedicated test modules for:

- Grid operations
- Coordinate utilities
- Puzzle generation
- Solver correctness
- Validator correctness
- Performance benchmarking

---

## 🌟 Why This Project Stands Out

Most Sudoku projects stop after implementing a solver.

This project demonstrates:

- Algorithm design
- Backtracking optimization
- Constraint satisfaction techniques
- Object-oriented design
- Test-driven development practices
- Interactive terminal application development

The result is a complete, production-style C++ project that showcases both software engineering principles and algorithmic problem-solving skills.

---

## 🛣️ Roadmap

- [ ] Persistent high-score tracking
- [ ] Best-time leaderboard
- [ ] Timer pause / resume
- [ ] ANSI colour toggle
- [ ] Puzzle difficulty rating
- [ ] Two-player race mode
- [ ] Save and resume games