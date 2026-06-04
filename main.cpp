#include <array>
#include <iostream>

#include "src/grid.h"
#include "src/sudoku_solver.h"

int main()
{
    try
    {
        std::array<std::array<int, 9>, 9> board{};

        std::cout
            << "Enter Sudoku puzzle (9 rows, use 0 for empty cells):\n";

        for (int row = 0; row < 9; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                std::cin >> board[row][col];
            }
        }

        sudoku::Grid puzzle(board);

        std::cout << "\nInput Puzzle:\n\n";
        std::cout << puzzle << '\n';

        sudoku::solve(puzzle);

        std::cout << "Solved Puzzle:\n\n";
        std::cout << puzzle << '\n';
    }
    catch (const std::exception& ex)
    {
        std::cerr
            << "Error: "
            << ex.what()
            << '\n';
    }

    return 0;
}