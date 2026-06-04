#ifndef TESTS_TEST_SUDOKU_GENERATOR_H_
#define TESTS_TEST_SUDOKU_GENERATOR_H_

#include <cassert>

#include "../src/sudoku_generator.h"

inline void test_generated_puzzles_have_unique_solution()
{
    for(int i=0;i<20;i++)
    {
        auto puzzle =
            sudoku::generate_puzzle();

        assert(
            sudoku::has_unique_solution(
                puzzle
            )
        );
    }
}

inline void test_generated_puzzles_are_not_identical()
{
    auto first =
        sudoku::generate_puzzle();

    bool found_different =
        false;

    for(int i=0;i<10;i++)
    {
        auto next =
            sudoku::generate_puzzle();

        if(!(first == next))
        {
            found_different = true;
            break;
        }
    }

    assert(found_different);
}

#endif