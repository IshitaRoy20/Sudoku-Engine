#ifndef SRC_SUDOKU_VALIDATOR_H_
#define SRC_SUDOKU_VALIDATOR_H_

#include "grid.h"

namespace sudoku {

inline bool is_valid_solution(
    const Grid& grid
)
{
    for(int row = 0; row < GRID_LEN; row++)
    {
        bool seen[10] = {};

        for(int col = 0; col < GRID_LEN; col++)
        {
            int value =
                grid.get({row,col});

            if(value < 1 || value > 9)
                return false;

            if(seen[value])
                return false;

            seen[value] = true;
        }
    }

    for(int col = 0; col < GRID_LEN; col++)
    {
        bool seen[10] = {};

        for(int row = 0; row < GRID_LEN; row++)
        {
            int value =
                grid.get({row,col});

            if(seen[value])
                return false;

            seen[value] = true;
        }
    }

    for(int boxRow = 0;
        boxRow < GRID_LEN;
        boxRow += 3)
    {
        for(int boxCol = 0;
            boxCol < GRID_LEN;
            boxCol += 3)
        {
            bool seen[10] = {};

            for(int r = boxRow;
                r < boxRow + 3;
                r++)
            {
                for(int c = boxCol;
                    c < boxCol + 3;
                    c++)
                {
                    int value =
                        grid.get({r,c});

                    if(seen[value])
                        return false;

                    seen[value] = true;
                }
            }
        }
    }

    return true;
}

}

#endif