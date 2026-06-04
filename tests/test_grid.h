#ifndef TESTS_TEST_GRID_H_
#define TESTS_TEST_GRID_H_

#include <cassert>

#include "../src/grid.h"

inline void test_grid_initialization()
{
    sudoku::Grid grid;

    for(int r=0;r<9;r++)
    {
        for(int c=0;c<9;c++)
        {
            assert(
                grid.get({r,c}) == 0
            );
        }
    }
}

inline void test_grid_update_and_get()
{
    sudoku::Grid grid;

    for(int r=0;r<9;r++)
    {
        for(int c=0;c<9;c++)
        {
            int value =
                (r+c)%9 + 1;

            grid.update(
                {r,c},
                value
            );

            assert(
                grid.get({r,c})
                ==
                value
            );
        }
    }
}

inline void test_possible_values_are_valid()
{
    sudoku::Grid grid;

    auto values =
        grid.get_possible_values(
            {0,0}
        );

    assert(
        values.size() == 9
    );

    for(int value : values)
    {
        assert(
            value >= 1 &&
            value <= 9
        );
    }
}

#endif