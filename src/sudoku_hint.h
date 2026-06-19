#ifndef SRC_SUDOKU_HINT_H_
#define SRC_SUDOKU_HINT_H_

#include <optional>

#include "grid.h"

namespace sudoku {

struct Hint {
    Coord cell;
    int   value;
};

// Finds the first empty cell in the puzzle and fills it from the solution.
// Returns the Hint so the caller can display it; mutates puzzle in-place.
inline std::optional<Hint> give_hint(Grid& puzzle, const Grid& solution)
{
    for (int r = 0; r < GRID_LEN; r++) {
        for (int c = 0; c < GRID_LEN; c++) {
            Coord cell{r, c};
            if (puzzle.get(cell) == 0) {
                int value = solution.get(cell);
                puzzle.update(cell, value);
                return Hint{cell, value};
            }
        }
    }
    return std::nullopt;
}

} 

#endif