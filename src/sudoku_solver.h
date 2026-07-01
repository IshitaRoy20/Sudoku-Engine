#ifndef SRC_SUDOKU_SOLVER_H_
#define SRC_SUDOKU_SOLVER_H_

#include <array>
#include <chrono>
#include <limits>
#include <stdexcept>
#include <string>

#include "grid.h"

namespace sudoku {

struct SolveStats {

    int backtracks = 0;
    std::string solved_by;
    double time_ms = 0.0;
};

using CandidateBoard = std::array<std::array<uint16_t, 9>, 9>;

inline uint16_t bit(int v)
{
    return static_cast<uint16_t>(1 << v);
}

inline int popcount(uint16_t mask)
{
    int n = 0;
    while (mask) { n += (mask & 1); mask >>= 1; }
    return n;
}

inline int lowest_bit(uint16_t mask)
{
    for (int v = 1; v <= 9; v++)
        if (mask & bit(v)) return v;
    return 0;
}


inline CandidateBoard build_candidates(const Grid& grid)
{
    CandidateBoard cands{};
    constexpr uint16_t ALL = 0b1111111110; // bits 1-9 set

    for (int r = 0; r < GRID_LEN; r++)
        for (int c = 0; c < GRID_LEN; c++)
            cands[r][c] = grid.is_empty({r, c}) ? ALL : 0;

    for (int r = 0; r < GRID_LEN; r++) {
        for (int c = 0; c < GRID_LEN; c++) {
            int v = grid.get({r, c});
            if (v == 0) continue;
            uint16_t b = bit(v);

            for (int col = 0; col < GRID_LEN; col++) cands[r][col]   &= ~b;
            for (int row = 0; row < GRID_LEN; row++) cands[row][c]   &= ~b;

            int br = (r / 3) * 3, bc = (c / 3) * 3;
            for (int dr = 0; dr < 3; dr++)
                for (int dc = 0; dc < 3; dc++)
                    cands[br+dr][bc+dc] &= ~b;

            cands[r][c] = 0; // filled cell has no candidates
        }
    }
    return cands;
}



inline bool eliminate(CandidateBoard& cands, const Grid& grid,
                      int r, int c, int v)
{
    uint16_t b = bit(v);

    // Row peers
    for (int col = 0; col < GRID_LEN; col++) {
        if (col == c || !(cands[r][col] & b)) continue;
        cands[r][col] &= ~b;
        if (cands[r][col] == 0 && grid.is_empty({r, col})) return false;
    }

    // Column peers
    for (int row = 0; row < GRID_LEN; row++) {
        if (row == r || !(cands[row][c] & b)) continue;
        cands[row][c] &= ~b;
        if (cands[row][c] == 0 && grid.is_empty({row, c})) return false;
    }

    // Box peers
    int br = (r / 3) * 3, bc = (c / 3) * 3;
    for (int dr = 0; dr < 3; dr++) {
        for (int dc = 0; dc < 3; dc++) {
            int pr = br+dr, pc = bc+dc;
            if ((pr == r && pc == c) || !(cands[pr][pc] & b)) continue;
            cands[pr][pc] &= ~b;
            if (cands[pr][pc] == 0 && grid.is_empty({pr, pc})) return false;
        }
    }
    return true;
}


inline bool place(Grid& grid, CandidateBoard& cands, int r, int c, int v)
{
    grid.update({r, c}, v);
    cands[r][c] = 0;
    return eliminate(cands, grid, r, c, v);
}

inline bool apply_naked_singles(Grid& grid, CandidateBoard& cands,
                                bool& contradiction)
{
    bool placed_any = false;
    for (int r = 0; r < GRID_LEN; r++) {
        for (int c = 0; c < GRID_LEN; c++) {
            if (!grid.is_empty({r, c})) continue;
            if (popcount(cands[r][c]) != 1) continue;

            int v = lowest_bit(cands[r][c]);
            if (!place(grid, cands, r, c, v)) {
                contradiction = true;
                return false;
            }
            placed_any = true;
        }
    }
    return placed_any;
}

inline bool apply_hidden_singles(Grid& grid, CandidateBoard& cands,
                                 bool& contradiction)
{
    bool placed_any = false;

    // ── Rows ─────────────────────────────────────────────────
    for (int r = 0; r < GRID_LEN; r++) {
        for (int v = 1; v <= 9; v++) {
            int count = 0, last_c = -1;
            for (int c = 0; c < GRID_LEN; c++) {
                if (!grid.is_empty({r, c})) continue;
                if (cands[r][c] & bit(v)) { count++; last_c = c; }
            }
            if (count != 1) continue;
            if (!place(grid, cands, r, last_c, v)) {
                contradiction = true; return false;
            }
            placed_any = true;
        }
    }

    // ── Columns ───────────────────────────────────────────────
    for (int c = 0; c < GRID_LEN; c++) {
        for (int v = 1; v <= 9; v++) {
            int count = 0, last_r = -1;
            for (int r = 0; r < GRID_LEN; r++) {
                if (!grid.is_empty({r, c})) continue;
                if (cands[r][c] & bit(v)) { count++; last_r = r; }
            }
            if (count != 1) continue;
            if (!place(grid, cands, last_r, c, v)) {
                contradiction = true; return false;
            }
            placed_any = true;
        }
    }

    // ── 3x3 Boxes ─────────────────────────────────────────────
    for (int br = 0; br < 9; br += 3) {
        for (int bc = 0; bc < 9; bc += 3) {
            for (int v = 1; v <= 9; v++) {
                int count = 0, last_r = -1, last_c = -1;
                for (int dr = 0; dr < 3; dr++) {
                    for (int dc = 0; dc < 3; dc++) {
                        int r = br+dr, c = bc+dc;
                        if (!grid.is_empty({r, c})) continue;
                        if (cands[r][c] & bit(v)) {
                            count++; last_r = r; last_c = c;
                        }
                    }
                }
                if (count != 1) continue;
                if (!place(grid, cands, last_r, last_c, v)) {
                    contradiction = true; return false;
                }
                placed_any = true;
            }
        }
    }

    return placed_any;
}


inline bool propagate(Grid& grid, CandidateBoard& cands)
{
    while (true) {
        bool progress     = false;
        bool contradiction = false;

        while (apply_naked_singles(grid, cands, contradiction)) {
            if (contradiction) return false;
            progress = true;
        }
        if (contradiction) return false;

        bool h = apply_hidden_singles(grid, cands, contradiction);
        if (contradiction) return false;
        if (h) { progress = true; continue; }

        if (!progress) break;
    }
    return true;
}
inline bool solve_recursive(Grid& grid, CandidateBoard& cands,
                             int& backtrack_count)
{
    // Find the empty cell with fewest candidates (MRV)
    int   best_count = 10;
    Coord best_cell  = {-1, -1};

    for (int r = 0; r < GRID_LEN; r++) {
        for (int c = 0; c < GRID_LEN; c++) {
            if (!grid.is_empty({r, c})) continue;
            int cnt = popcount(cands[r][c]);
            if (cnt == 0) return false; // contradiction
            if (cnt < best_count) { best_count = cnt; best_cell = {r, c}; }
        }
    }

    if (best_cell.first == -1) return true; // all cells filled 

    int r = best_cell.first;
    int c = best_cell.second;

    uint16_t mask = cands[r][c];
    for (int v = 1; v <= 9; v++) {
        if (!(mask & bit(v))) continue;

        Grid           grid_snap  = grid;
        CandidateBoard cands_snap = cands;

        if (place(grid, cands, r, c, v)) {
            if (propagate(grid, cands)) {
                if (solve_recursive(grid, cands, backtrack_count))
                    return true;
            }
        }

        // This attempt failed — count it and restore
        ++backtrack_count;
        grid  = grid_snap;
        cands = cands_snap;
    }
    return false;
}

inline void solve(Grid& grid)
{
    CandidateBoard cands = build_candidates(grid);

    if (!propagate(grid, cands))
        throw std::logic_error("Puzzle has no solution.");

    // Check if propagation alone solved it
    bool needs_bt = false;
    for (int r = 0; r < GRID_LEN && !needs_bt; r++)
        for (int c = 0; c < GRID_LEN && !needs_bt; c++)
            if (grid.is_empty({r, c})) needs_bt = true;

    if (!needs_bt) return;

    int dummy = 0;
    if (!solve_recursive(grid, cands, dummy))
        throw std::logic_error("Puzzle has no solution.");
}

inline SolveStats solve_with_stats(Grid& grid)
{
    SolveStats stats;
    auto t0 = std::chrono::high_resolution_clock::now();

    CandidateBoard cands = build_candidates(grid);

    if (!propagate(grid, cands)) {
        stats.solved_by = "unsolvable";
        auto t1 = std::chrono::high_resolution_clock::now();
        stats.time_ms = std::chrono::duration<double, std::milli>(t1-t0).count();
        throw std::logic_error("Puzzle has no solution.");
    }

    // Check if propagation alone solved it
    bool needs_bt = false;
    for (int r = 0; r < GRID_LEN && !needs_bt; r++)
        for (int c = 0; c < GRID_LEN && !needs_bt; c++)
            if (grid.is_empty({r, c})) needs_bt = true;

    if (!needs_bt) {
        stats.solved_by = "propagation";
    } else {
        if (!solve_recursive(grid, cands, stats.backtracks))
            throw std::logic_error("Puzzle has no solution.");
        stats.solved_by = "backtracking";
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    stats.time_ms = std::chrono::duration<double, std::milli>(t1-t0).count();
    return stats;
}

inline bool baseline_recursive(Grid& grid, int& backtrack_count)
{
    // MRV: find empty cell with fewest possible values
    int   best_count = 10;
    Coord best_cell  = {-1, -1};

    for (int r = 0; r < GRID_LEN; r++) {
        for (int c = 0; c < GRID_LEN; c++) {
            if (!grid.is_empty({r, c})) continue;
            auto cands = grid.get_possible_values({r, c});
            if (cands.empty()) return false;
            if ((int)cands.size() < best_count) {
                best_count = (int)cands.size();
                best_cell  = {r, c};
            }
        }
    }

    if (best_cell.first == -1) return true;

    auto candidates = grid.get_possible_values(best_cell);
    for (int v : candidates) {
        grid.update(best_cell, v);
        if (baseline_recursive(grid, backtrack_count)) return true;
        ++backtrack_count;
        grid.update(best_cell, 0);
    }
    return false;
}

inline SolveStats solve_baseline(Grid& grid)
{
    SolveStats stats;
    stats.solved_by = "backtracking-only";
    auto t0 = std::chrono::high_resolution_clock::now();

    if (!baseline_recursive(grid, stats.backtracks))
        throw std::logic_error("Puzzle has no solution.");

    auto t1 = std::chrono::high_resolution_clock::now();
    stats.time_ms = std::chrono::duration<double, std::milli>(t1-t0).count();
    return stats;
}

}
#endif