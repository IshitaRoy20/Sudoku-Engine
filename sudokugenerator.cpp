#include <iostream>

#include "src/sudoku_generator.h"
#include "src/sudoku_solver.h"
#include "src/sudoku_hint.h"

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

    sudoku::Grid puzzle =
        sudoku::generate_puzzle(
            difficulty
        );

    sudoku::Grid solution =
        puzzle;

    sudoku::solve(
        solution
    );

    std::cout
        << "\nGenerated Puzzle:\n\n";

    std::cout
        << puzzle
        << '\n';

    int hints_used = 0;

    while(hints_used < 2)
    {
        char want_hint;

        std::cout
            << "\nDo you want a hint? (y/n): ";

        std::cin
            >> want_hint;

        if(
            want_hint != 'y' &&
            want_hint != 'Y'
        )
        {
            break;
        }

        sudoku::give_hint(
            puzzle,
            solution
        );

        hints_used++;

        std::cout
            << "\nHint "
            << hints_used
            << "/2 used\n\n";

        std::cout
            << puzzle
            << '\n';
    }

    if(hints_used == 2)
    {
        std::cout
            << "\nNo more hints available.\n";
    }

    char reveal_solution;

    std::cout
        << "\nDo you want the complete solution? (y/n): ";

    std::cin
        >> reveal_solution;

    if(
        reveal_solution == 'y' ||
        reveal_solution == 'Y'
    )
    {
        std::cout
            << "\nSolved Sudoku:\n\n";

        std::cout
            << solution
            << '\n';
    }

    return 0;
}