#ifndef SRC_SUDOKU_HINT_H_
#define SRC_SUDOKU_HINT_H_

#include "grid.h"

namespace sudoku {

inline bool give_hint(
    Grid& puzzle,
    const Grid& solution
)
{
    for(int r = 0; r < GRID_LEN; r++)
    {
        for(int c = 0; c < GRID_LEN; c++)
        {
            Coord cell{r,c};

            if(
                puzzle.get(cell) == 0
            )
            {
                puzzle.update(
                    cell,
                    solution.get(cell)
                );

                return true;
            }
        }
    }

    return false;
}

}

#endif