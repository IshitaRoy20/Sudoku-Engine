#ifndef SRC_SUDOKU_GENERATOR_H_
#define SRC_SUDOKU_GENERATOR_H_

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include "grid.h"

namespace sudoku {

enum class Difficulty { EASY, MEDIUM, HARD };

inline std::mt19937& global_rng()
{
    static std::mt19937 rng(
        std::chrono::steady_clock::now().time_since_epoch().count());
    return rng;
}

// Fill a complete, valid board

inline bool fill_complete_board(Grid& grid)
{
    Coord best_cell;
    bool  found     = false;
    int   best_size = 100;

    for (int row = 0; row < GRID_LEN; row++) {
        for (int col = 0; col < GRID_LEN; col++) {
            Coord current{row, col};
            if (!grid.is_empty(current)) continue;

            auto candidates = grid.get_possible_values(current);
            if (candidates.empty()) return false;

            if (static_cast<int>(candidates.size()) < best_size) {
                best_size = static_cast<int>(candidates.size());
                best_cell = current;
                found     = true;
            }
        }
    }

    if (!found) return true;   // board is full

    auto candidates = grid.get_possible_values(best_cell);
    std::shuffle(candidates.begin(), candidates.end(), global_rng());

    for (int value : candidates) {
        grid.update(best_cell, value);
        if (fill_complete_board(grid)) return true;
        grid.update(best_cell, 0);
    }

    return false;
}

// Solution counter

inline void count_solutions(Grid& grid, int& solutions, int limit = 2)
{
    if (solutions >= limit) return;

    Coord best_cell;
    bool  found     = false;
    int   best_size = 100;

    for (int row = 0; row < GRID_LEN; row++) {
        for (int col = 0; col < GRID_LEN; col++) {
            Coord current{row, col};
            if (!grid.is_empty(current)) continue;

            auto candidates = grid.get_possible_values(current);
            if (candidates.empty()) return;

            if (static_cast<int>(candidates.size()) < best_size) {
                best_size = static_cast<int>(candidates.size());
                best_cell = current;
                found     = true;
            }
        }
    }

    if (!found) { solutions++; return; }

    auto candidates = grid.get_possible_values(best_cell);
    for (int value : candidates) {
        grid.update(best_cell, value);
        count_solutions(grid, solutions, limit);
        grid.update(best_cell, 0);
    }
}

inline bool has_unique_solution(Grid puzzle)
{
    int solutions = 0;
    count_solutions(puzzle, solutions, 2);
    return solutions == 1;
}

//  Difficulty -> cells to remove 

inline int cells_to_remove(Difficulty difficulty)
{
    switch (difficulty) {
        case Difficulty::EASY:   return 35;
        case Difficulty::MEDIUM: return 45;
        case Difficulty::HARD:   return 55;
    }
    return 45;
}

//  Main generation entry point 

inline Grid generate_puzzle(Difficulty difficulty = Difficulty::MEDIUM)
{
    Grid puzzle;
    fill_complete_board(puzzle);

    int remove_count = cells_to_remove(difficulty);

    std::vector<Coord> cells;
    cells.reserve(GRID_LEN * GRID_LEN);
    for (int r = 0; r < GRID_LEN; r++)
        for (int c = 0; c < GRID_LEN; c++)
            cells.push_back({r, c});

    std::shuffle(cells.begin(), cells.end(), global_rng());

    int removed = 0;
    for (const auto& cell : cells) {
        if (removed >= remove_count) break;

        int old_value = puzzle.get(cell);
        puzzle.update(cell, 0);

        if (!has_unique_solution(puzzle))
            puzzle.update(cell, old_value);
        else
            removed++;
    }

    return puzzle;
}

} // namespace sudoku

#endif