#ifndef SRC_COORD_H_
#define SRC_COORD_H_

#include <utility>

namespace sudoku {

inline constexpr int GRID_LEN = 9;

// Coord is a (row, col) pair, 0-indexed
using Coord = std::pair<int, int>;

}

#endif