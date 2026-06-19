#ifndef SRC_SUDOKU_SOLVER_H_
#define SRC_SUDOKU_SOLVER_H_

#include <limits>
#include <stdexcept>

#include "grid.h"

namespace sudoku {

inline bool solve_recursive(Grid& grid)
{
    Coord best_cell;
    bool  found           = false;
    int   best_candidates = std::numeric_limits<int>::max();

    for (int row = 0; row < GRID_LEN; row++) {
        for (int col = 0; col < GRID_LEN; col++) {
            Coord current{row, col};
            if (!grid.is_empty(current)) continue;

            auto candidates = grid.get_possible_values(current);
            if (candidates.empty()) return false;

            if (static_cast<int>(candidates.size()) < best_candidates) {
                best_candidates = static_cast<int>(candidates.size());
                best_cell       = current;
                found           = true;
            }
        }
    }

    if (!found) return true; 

    auto candidates = grid.get_possible_values(best_cell);
    for (int value : candidates) {
        grid.update(best_cell, value);
        if (solve_recursive(grid)) return true;
        grid.update(best_cell, 0);
    }

    return false;
}

inline void solve(Grid& grid)
{
    if (!solve_recursive(grid))
        throw std::logic_error("Puzzle has no solution.");
}

} 

#endif