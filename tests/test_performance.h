#ifndef TESTS_TEST_PERFORMANCE_H_
#define TESTS_TEST_PERFORMANCE_H_

#include <chrono>
#include <iostream>

#include "../src/sudoku_generator.h"
#include "../src/sudoku_solver.h"

inline void benchmark_solver()
{
    auto start =
        std::chrono::high_resolution_clock::now();

    for(int i=0;i<100;i++)
    {
        auto puzzle =
            sudoku::generate_puzzle();

        sudoku::solve(
            puzzle
        );
    }

    auto end =
        std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(end-start);

    std::cout
        << "100 puzzles solved in "
        << duration.count()
        << " ms\n";
}

#endif