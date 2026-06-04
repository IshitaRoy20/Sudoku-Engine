#ifndef SRC_SUDOKU_HINT_H_
#define SRC_SUDOKU_HINT_H_

#include <optional>

#include "grid.h"

namespace sudoku {

struct Hint
{
    Coord cell;
    int value;
};

inline std::optional<Hint> give_hint(
    Grid& puzzle,
    const Grid& solution
)
{
    for(int r = 0; r < GRID_LEN; r++)
    {
        for(int c = 0; c < GRID_LEN; c++)
        {
            Coord cell{r,c};

            if(puzzle.get(cell) == 0)
            {
                int value =
                    solution.get(cell);

                puzzle.update(
                    cell,
                    value
                );

                return Hint{
                    cell,
                    value
                };
            }
        }
    }

    return std::nullopt;
}

}

#endif