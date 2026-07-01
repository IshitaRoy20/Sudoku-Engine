cmake_minimum_required(VERSION 3.15)
project(SudokuEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

# ── Main game ────────────────────────────────────────────────────
add_executable(sudoku main.cpp)
target_include_directories(sudoku PRIVATE ${CMAKE_SOURCE_DIR})

# ── Benchmark ────────────────────────────────────────────────────
# Build with Release for accurate timings (O2 optimisations)
# Usage: cmake -B build -DCMAKE_BUILD_TYPE=Release
#        cmake --build build
#        ./build/benchmark
add_executable(benchmark benchmark.cpp)
target_include_directories(benchmark PRIVATE ${CMAKE_SOURCE_DIR})

# ── Test suite ───────────────────────────────────────────────────
#add_executable(test_solver tests/test_solver.cpp)
#target_include_directories(test_solver PRIVATE ${CMAKE_SOURCE_DIR})

#enable_testing()
#add_test(NAME SolverTests COMMAND test_solver)