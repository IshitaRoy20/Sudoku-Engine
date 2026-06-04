#ifndef TESTS_TEST_COORD_UTILS_H_
#define TESTS_TEST_COORD_UTILS_H_

#include <cassert>
#include <utility>

#include "../src/coord_utils.h"

inline void test_successive_cell_coords_are_generated_correctly()
{
    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            auto next =
                sudoku::get_next_cell_coord(
                    std::make_pair(row, col)
                );

            assert(
                next ==
                std::make_pair(row, col + 1)
            );
        }
    }

    for(int row = 0; row < 8; row++)
    {
        auto next =
            sudoku::get_next_cell_coord(
                std::make_pair(row, 8)
            );

        assert(
            next ==
            std::make_pair(row + 1, 0)
        );
    }

    assert(
        sudoku::get_next_cell_coord(
            std::make_pair(8, 8)
        )
        ==
        std::make_pair(8, 8)
    );
}

#endif