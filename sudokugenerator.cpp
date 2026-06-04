#include <iostream>

#include "src/sudoku_generator.h"

int main()
{
    int choice;

    std::cout << "Choose Difficulty\n";
    std::cout << "1. Easy\n";
    std::cout << "2. Medium\n";
    std::cout << "3. Hard\n";
    std::cout << "\nChoice: ";

    std::cin >> choice;

    sudoku::Difficulty difficulty;

    switch(choice)
    {
        case 1:
            difficulty =
                sudoku::Difficulty::EASY;
            break;

        case 2:
            difficulty =
                sudoku::Difficulty::MEDIUM;
            break;

        case 3:
            difficulty =
                sudoku::Difficulty::HARD;
            break;

        default:
            difficulty =
                sudoku::Difficulty::MEDIUM;
    }

    auto puzzle =
        sudoku::generate_puzzle(
            difficulty
        );

    std::cout
        << "\nGenerated Puzzle:\n\n";

    std::cout
        << puzzle
        << '\n';

    return 0;
}