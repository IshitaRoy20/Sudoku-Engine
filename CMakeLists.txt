cmake_minimum_required(VERSION 3.15)
project(SudokuEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Strict warnings — catch issues at compile time
add_compile_options(-Wall -Wextra -Wpedantic)

# ── Main game (replaces both old binaries) ─────────────────
add_executable(sudoku main.cpp)
target_include_directories(sudoku PRIVATE ${CMAKE_SOURCE_DIR})

# ── Test suite ─────────────────────────────────────────────
add_executable(testsuite tests/test_sudoku_suite.cpp)
target_include_directories(testsuite PRIVATE ${CMAKE_SOURCE_DIR})

# Enable ctest
enable_testing()
add_test(NAME SudokuTests COMMAND testsuite)