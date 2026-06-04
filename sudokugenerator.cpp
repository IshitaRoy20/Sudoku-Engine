#include <iostream>
#include <array>

#include "src/sudoku_generator.h"
#include "src/sudoku_solver.h"
#include "src/sudoku_validator.h"
#include "src/sudoku_hint.h"

int main()
{
    int difficulty_choice;

    std::cout
        << "Choose Difficulty\n";

    std::cout
        << "1. Easy\n";

    std::cout
        << "2. Medium\n";

    std::cout
        << "3. Hard\n";

    std::cout
        << "\nChoice: ";

    std::cin
        >> difficulty_choice;

    sudoku::Difficulty difficulty;

    switch(difficulty_choice)
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

    sudoku::Grid original_puzzle =
        puzzle;

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

    while(true)
    {
        if(hints_used < 2)
        {
            std::cout
                << "\nChoose an option:\n";

            std::cout
                << "1. Solve it myself\n";

            std::cout
                << "2. Get a hint\n";

            std::cout
                << "\nChoice: ";

            int choice;
            std::cin >> choice;

            if(choice == 1)
            {
                break;
            }

            if(choice == 2)
            {
                auto hint =
                    sudoku::give_hint(
                        puzzle,
                        solution
                    );

                if(hint)
                {
                    hints_used++;

                    std::cout
                        << "\nHint "
                        << hints_used
                        << "/2\n";

                    std::cout
                        << "Row "
                        << hint->cell.first + 1
                        << ", Column "
                        << hint->cell.second + 1
                        << " = "
                        << hint->value
                        << "\n\n";

                    std::cout
                        << puzzle
                        << '\n';
                }

                if(hints_used == 2)
                {
                    std::cout
                        << "\nNo more hints available.\n";

                    break;
                }
            }
        }
    }

    if(hints_used == 2)
    {
        std::cout
            << "\nChoose an option:\n";

        std::cout
            << "1. Solve it myself\n";

        std::cout
            << "2. Show solution\n";

        std::cout
            << "\nChoice: ";

        int choice;
        std::cin >> choice;

        if(choice == 2)
        {
            std::cout
                << "\nSolved Sudoku:\n\n";

            std::cout
                << solution
                << '\n';

            return 0;
        }
    }

    std::cout
        << "\nEnter your completed Sudoku "
        << "(81 numbers):\n";

    std::array<
        std::array<int,9>,
        9
    > user_board{};

    for(int r = 0; r < 9; r++)
    {
        for(int c = 0; c < 9; c++)
        {
            std::cin
                >> user_board[r][c];
        }
    }

    sudoku::Grid user_solution(
        user_board
    );

    bool respects_clues = true;

    for(int r = 0; r < sudoku::GRID_LEN; r++)
    {
        for(int c = 0; c < sudoku::GRID_LEN; c++)
        {
            sudoku::Coord cell{r,c};

            if(
                original_puzzle.get(cell) != 0 &&
                original_puzzle.get(cell)
                != user_solution.get(cell)
            )
            {
                respects_clues = false;
            }
        }
    }

    if(
        respects_clues &&
        sudoku::is_valid_solution(
            user_solution
        )
    )
    {
        std::cout
            << "\nCorrect Solution! 🎉\n";
    }
    else
    {
        std::cout
            << "\nIncorrect Solution ❌\n";

        std::cout
            << "\nActual Solution:\n\n";

        std::cout
            << solution
            << '\n';
    }

    return 0;
}