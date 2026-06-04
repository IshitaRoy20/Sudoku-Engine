#include <iostream>

#include "test_coord_utils.h"
#include "test_grid.h"
#include "test_sudoku_solver.h"
#include "test_sudoku_validator.h"
#include "test_sudoku_generator.h"
#include "test_performance.h"

int main()
{
    test_successive_cell_coords_are_generated_correctly();
    std::cout << "[PASS] Coord Utils\n";

    test_grid_initialization();
    test_grid_update_and_get();
    test_possible_values_are_valid();
    std::cout << "[PASS] Grid\n";

    test_generated_puzzles_can_be_solved();
    std::cout << "[PASS] Solver\n";

    test_validator_accepts_valid_solutions();
    std::cout << "[PASS] Validator\n";

    test_generated_puzzles_have_unique_solution();
    test_generated_puzzles_are_not_identical();
    std::cout << "[PASS] Generator\n";

    benchmark_solver();

    std::cout
        << "\nAll tests passed.\n";

    return 0;
}