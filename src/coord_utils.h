#ifndef SRC_COORD_UTILS_H_
#define SRC_COORD_UTILS_H_

#include <chrono>
#include <random>
#include <set>

#include "coord.h"

namespace sudoku {
inline Coord get_next_cell_coord(Coord coord)
{
    if(coord.first == GRID_LEN-1 &&
       coord.second == GRID_LEN-1)
    {
        return coord;
    }

    if(coord.second == GRID_LEN-1)
    {
        return {
            coord.first + 1,
            0
        };
    }

    return {
        coord.first,
        coord.second + 1
    };
}
inline std::set<Coord>
get_random_cells(int count)
{
    std::set<Coord> cells;

    std::mt19937 rng(
        std::chrono::steady_clock::now()
        .time_since_epoch()
        .count()
    );

    std::uniform_int_distribution<>
        dist(0, GRID_LEN - 1);

    while(
        static_cast<int>(cells.size())
        < count
    )
    {
        cells.insert({
            dist(rng),
            dist(rng)
        });
    }

    return cells;
}
}
#endif