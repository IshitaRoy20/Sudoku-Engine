#include <iostream>

#include "src/sudoku_generator.h"

int main()
{
    try
    {
        sudoku::Grid puzzle =
            sudoku::generate_puzzle(
               // sudoku::Difficulty::MEDIUM
            );

        std::cout
            << "Generated Sudoku Puzzle:\n\n";

        std::cout
            << puzzle
            << '\n';
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