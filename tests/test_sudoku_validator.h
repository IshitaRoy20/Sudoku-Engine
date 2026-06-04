#ifndef TESTS_TEST_SUDOKU_VALIDATOR_H_
#define TESTS_TEST_SUDOKU_VALIDATOR_H_

#include <cassert>

#include "../src/sudoku_generator.h"
#include "../src/sudoku_solver.h"
#include "../src/sudoku_validator.h"

inline void test_validator_accepts_valid_solutions()
{
    for(int i=0;i<20;i++)
    {
        auto puzzle =
            sudoku::generate_puzzle();

        sudoku::solve(
            puzzle
        );

        assert(
            sudoku::is_valid_solution(
                puzzle
            )
        );
    }
}

#endif