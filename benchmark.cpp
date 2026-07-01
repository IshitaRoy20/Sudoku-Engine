#include <algorithm>
#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "src/grid.h"
#include "src/sudoku_generator.h"
#include "src/sudoku_solver.h"
#include "src/sudoku_validator.h"

static constexpr int PUZZLES_PER_DIFFICULTY = 1000;

// ── Result for one solver on one difficulty ───────────────────────
struct BenchResult {
    std::string solver_name;
    double      avg_time_ms   = 0.0;
    double      max_time_ms   = 0.0;
    double      avg_backtracks = 0.0;
    int         max_backtracks = 0;
    int         solved_by_propagation = 0;  // count that needed zero backtracks
    int         total          = 0;
};

enum class SolverType { BASELINE, PROPAGATION };

BenchResult run_benchmark(const std::vector<sudoku::Grid>& puzzles,
                          SolverType solver,
                          const std::string& name)
{
    BenchResult res;
    res.solver_name = name;
    res.total       = static_cast<int>(puzzles.size());

    double total_time   = 0.0;
    long   total_bt     = 0;

    for (const sudoku::Grid& puzzle : puzzles) {
        sudoku::Grid g = puzzle; 
        sudoku::SolveStats stats;

        try {
            if (solver == SolverType::BASELINE)
                stats = sudoku::solve_baseline(g);
            else
                stats = sudoku::solve_with_stats(g);
        } catch (...) {
            res.total--;
            continue;
        }

        total_time       += stats.time_ms;
        total_bt         += stats.backtracks;
        res.max_time_ms   = std::max(res.max_time_ms, stats.time_ms);
        res.max_backtracks= std::max(res.max_backtracks, stats.backtracks);

        if (stats.backtracks == 0)
            res.solved_by_propagation++;
    }

    if (res.total > 0) {
        res.avg_time_ms    = total_time / res.total;
        res.avg_backtracks = static_cast<double>(total_bt) / res.total;
    }

    return res;
}

static void separator(char c = '-', int width = 72)
{
    std::cout << std::string(width, c) << "\n";
}

static void print_table(const std::string& difficulty_name,
                        const BenchResult& baseline,
                        const BenchResult& propagation)
{
    const int W0 = 24; // solver name
    const int W1 = 10; // avg time
    const int W2 = 10; // max time
    const int W3 = 12; // avg backtracks
    const int W4 = 10; // max backtracks
    const int W5 = 12; // zero-backtrack%

    separator('=');
    std::cout << "  " << difficulty_name
              << "  (" << baseline.total << " puzzles)\n";
    separator();

    // Header
    std::cout
        << std::left  << std::setw(W0) << "  Solver"
        << std::right << std::setw(W1) << "Avg ms"
        << std::right << std::setw(W2) << "Max ms"
        << std::right << std::setw(W3) << "Avg BT"
        << std::right << std::setw(W4) << "Max BT"
        << std::right << std::setw(W5) << "Pure logic"
        << "\n";
    separator();

    auto print_row = [&](const BenchResult& r) {
        double pct = r.total > 0
            ? 100.0 * r.solved_by_propagation / r.total
            : 0.0;
        std::cout
            << std::left  << std::setw(W0) << ("  " + r.solver_name)
            << std::right << std::fixed << std::setprecision(3)
            << std::setw(W1) << r.avg_time_ms
            << std::setw(W2) << r.max_time_ms
            << std::setprecision(1)
            << std::setw(W3) << r.avg_backtracks
            << std::setprecision(0)
            << std::setw(W4) << r.max_backtracks
            << std::setw(W5) << (std::to_string((int)pct) + "%")
            << "\n";
    };

    print_row(baseline);
    print_row(propagation);

    // Speedup line
    separator();
    if (baseline.avg_time_ms > 0.0 && propagation.avg_time_ms > 0.0) {
        double speedup = baseline.avg_time_ms / propagation.avg_time_ms;
        double bt_reduction = baseline.avg_backtracks > 0
            ? (1.0 - propagation.avg_backtracks / baseline.avg_backtracks) * 100.0
            : 0.0;
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "  Speedup:       " << speedup << "x faster\n";
        std::cout << "  BT reduction:  " << bt_reduction << "% fewer backtracks\n";
    }
    std::cout << "\n";
}

// ── Main ──────────────────────────────────────────────────────────
int main()
{
    separator('=');
    std::cout << "  Sudoku Engine — Solver Benchmark\n";
    std::cout << "  " << PUZZLES_PER_DIFFICULTY
              << " puzzles per difficulty, 2 solvers\n";
    std::cout << "  Solvers: Baseline (MRV backtracking only)\n";
    std::cout << "           Propagation (naked+hidden singles + MRV)\n";
    // std::cout << "  AC-3 slot: ready — implement solve_ac3() and add here\n";
    separator('=');
    std::cout << "\n";

    struct DifficultyEntry {
        sudoku::Difficulty diff;
        std::string        name;
    };

    const std::array<DifficultyEntry, 3> difficulties = {{
        { sudoku::Difficulty::EASY,   "EASY   (35 cells removed, ~46 clues)" },
        { sudoku::Difficulty::MEDIUM, "MEDIUM (45 cells removed, ~36 clues)" },
        { sudoku::Difficulty::HARD,   "HARD   (55 cells removed, ~26 clues)" },
    }};

    for (const auto& entry : difficulties) {
        std::cout << "  Generating " << PUZZLES_PER_DIFFICULTY
                  << " " << entry.name << " puzzles...\n";
        std::cout.flush();

        std::vector<sudoku::Grid> puzzles;
        puzzles.reserve(PUZZLES_PER_DIFFICULTY);

        for (int i = 0; i < PUZZLES_PER_DIFFICULTY; i++)
            puzzles.push_back(sudoku::generate_puzzle(entry.diff));

        std::cout << "  Running baseline solver...\n"; std::cout.flush();
        auto r_baseline = run_benchmark(puzzles, SolverType::BASELINE,
                                        "Baseline (BT only)");

        std::cout << "  Running propagation solver...\n"; std::cout.flush();
        auto r_prop     = run_benchmark(puzzles, SolverType::PROPAGATION,
                                        "Propagation (N+H+BT)");

        print_table(entry.name, r_baseline, r_prop);
    }

    separator('=');
    std::cout << "\n";
    std::cout << "  Backtracks = how many times the solver guessed wrong\n";
    std::cout << "  and had to undo. Zero = solved by pure logic alone.\n\n";
    std::cout << "  'Pure logic %' = puzzles solved with zero backtracks.\n";
    std::cout << "  Baseline: 0% (always guesses). Propagation: high % on easy.\n\n";
    separator('=');

    return 0;
}