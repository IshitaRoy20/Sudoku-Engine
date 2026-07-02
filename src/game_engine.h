#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "grid.h"
#include "sudoku_generator.h"
#include "sudoku_hint.h"
#include "sudoku_solver.h"
#include "sudoku_validator.h"

namespace sudoku {


inline constexpr int MAX_HINTS    = 3;  
inline constexpr int MAX_MISTAKES = 5; 
inline constexpr int GRID_SIZE    = 9;

// ANSI colour helpers
namespace ansi {
    inline const std::string RESET  = "\033[0m";
    inline const std::string BOLD   = "\033[1m";
    inline const std::string RED    = "\033[31m";
    inline const std::string GREEN  = "\033[32m";
    inline const std::string YELLOW = "\033[33m";
    inline const std::string CYAN   = "\033[36m";
    inline const std::string DIM    = "\033[2m";
}

//  Move: stored in undo stack 
struct Move {
    Coord cell;
    int   prev_value;   
    int   new_value;   
};

using CandidateSet = std::set<int>;
using PencilBoard  = std::array<std::array<CandidateSet, GRID_SIZE>, GRID_SIZE>;

class GameEngine {
public:
    static void run() {
        GameEngine engine;
        engine.main_menu();
    }

private:
    Grid          puzzle_;          
    Grid          original_;       
    Grid          solution_;       
    PencilBoard   pencil_marks_{};  
    std::stack<Move> undo_stack_;  

    int  hints_used_    = 0;
    int  mistakes_      = 0;
    bool game_over_     = false;
    bool puzzle_loaded_ = false;    

    using Clock     = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    TimePoint start_time_;


    void main_menu() {
        print_banner();
        while (true) {
            std::cout << ansi::BOLD
                      << "\n┌─── MAIN MENU ───────────────────────┐\n"
                      << "│  1. Play (generate puzzle)          │\n"
                      << "│  2. Solve a puzzle I type in        │\n"
                      << "│  3. Quit                            │\n"
                      << "└─────────────────────────────────────┘\n"
                      << ansi::RESET
                      << "Choice: ";

            int choice = read_int(1, 3);
            if (choice == 1) { play_mode();  }
            else if (choice == 2) { solve_mode(); }
            else { std::cout << "\nThanks for playing! 👋\n"; return; }
        }
    }

    //  MODE 1: generated puzzle
    void play_mode() {
        reset_state();
        Difficulty diff = choose_difficulty();
        std::cout << "\n  Generating puzzle...\n";

        puzzle_   = generate_puzzle(diff);
        original_ = puzzle_;
        solution_ = puzzle_;
        solve(solution_);        
        puzzle_loaded_ = true;
        start_time_    = Clock::now();

        game_loop();
    }

    //  MODE 2: user types 81 numbers, engine solves
    void solve_mode() {
        reset_state();
 
        std::cout
            << "\n" << ansi::CYAN << ansi::BOLD
            << "  ┌─── Enter Your Puzzle ──────────────────────────────┐\n"
            << "  │  Type or paste numbers — spaces and newlines are   │\n"
            << "  │  all treated the same. Use 0 for empty cells.      │\n"
            << "  │                                                    │\n"
            << "  │  You can enter one number at a time, one row at a  │\n"
            << "  │  time, or paste all 81 numbers at once.            │\n"
            << "  └────────────────────────────────────────────────────┘\n"
            << ansi::RESET << "\n";
 
        std::array<std::array<int, 9>, 9> board{};
        int collected = 0;  
        std::cout << "  Row 1/9 (or paste all 9 rows now) : ";
        std::cout.flush();
 
        while (collected < 81) {
            std::string token;
 
            if (!(std::cin >> token)) {
                std::cout << ansi::RED
                          << "\n  Input stream closed. Returning to menu.\n"
                          << ansi::RESET;
                // drain stream errors before returning
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return;
            }
 
            bool pure_digits = !token.empty();
            for (char ch : token)
                if (!std::isdigit(static_cast<unsigned char>(ch)))
                    { pure_digits = false; break; }
 
            if (!pure_digits) {
                int row_of_error = collected / 9 + 1;
                std::cout << ansi::RED
                          << "\n  ✗ \"" << token << "\" is not a number. "
                          << "Restarting from row " << row_of_error << ".\n"
                          << ansi::RESET;
                // Reset to start of current row
                collected = (collected / 9) * 9;
                std::cout << "  Row " << row_of_error << "/9 : ";
                std::cout.flush();
                continue;
            }
 
            int v = std::stoi(token);

            if (v < 0 || v > 9) {
                int row_of_error = collected / 9 + 1;
                std::cout << ansi::RED
                          << "\n  ✗ " << v << " is out of range (0-9). "
                          << "Restarting from row " << row_of_error << ".\n"
                          << ansi::RESET;
                collected = (collected / 9) * 9;
                std::cout << "  Row " << row_of_error << "/9 : ";
                std::cout.flush();
                continue;
            }

            int r = collected / 9;
            int c = collected % 9;
            board[r][c] = v;
            ++collected;
 
            if (collected % 9 == 0) {
                int row = collected / 9;   // 1-indexed row just completed
                std::cout << ansi::DIM
                          << "  ✓ Row " << row << ":";
                for (int col = 0; col < 9; ++col)
                    std::cout << "  " << board[row - 1][col];
                std::cout << ansi::RESET << "\n";
                std::cout.flush();
 
                // Prompt for the next row — including row 9
                // so the user sees "Row 9/9 :" before typing/pasting
                if (collected < 81) {
                    std::cout << "  Row " << (row + 1) << "/9 : ";
                    std::cout.flush();
                }
            }
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        try {
            puzzle_   = Grid(board);
            original_ = puzzle_;
            solution_ = puzzle_;
            solve(solution_);
            puzzle_loaded_ = true;
            start_time_    = Clock::now();
        } catch (const std::exception& e) {
            std::cout << ansi::RED
                      << "\n  ✗ Puzzle error: " << e.what()
                      << "\n  Check for duplicate clues and try again.\n"
                      << ansi::RESET;
            return;
        }
 
        std::cout << ansi::GREEN << "\n  ✓ Puzzle loaded!\n" << ansi::RESET;
        std::cout << "\n  Your puzzle:\n";
        print_board(puzzle_);
        std::cout << "  Solution:\n";
        print_board(solution_);
    }

    void game_loop() {
        while (!game_over_) {
            print_status_bar();
            print_board(puzzle_);
            print_controls();

            std::cout << "\nCommand: ";
            std::string cmd = read_command();

            if      (cmd == "p") cmd_place();
            else if (cmd == "e") cmd_erase();
            else if (cmd == "n") cmd_note();
            else if (cmd == "h") cmd_hint();
            else if (cmd == "u") cmd_undo();
            else if (cmd == "c") cmd_check();
            else if (cmd == "s") cmd_show_solution();
            else if (cmd == "q") { cmd_quit(); return; }
            else {
                std::cout << ansi::YELLOW
                          << "Unknown command. See controls below.\n"
                          << ansi::RESET;
            }

            if (is_complete()) {
                handle_win();
                return;
            }
            if (mistakes_ >= MAX_MISTAKES) {
                handle_loss();
                return;
            }
        }
    }

    void cmd_place() {
        Coord cell = read_coord();
        if (is_clue(cell)) {
            std::cout << ansi::RED << "  That cell is a given clue — can't edit it.\n" << ansi::RESET;
            return;
        }
        std::cout << "  Value (1-9): ";
        int val = read_int(1, 9);

        int prev = puzzle_.get(cell);
        if (solution_.get(cell) != val) {
            ++mistakes_;
            std::cout << ansi::RED
                      << "  ✗ Wrong! ("
                      << mistakes_ << "/" << MAX_MISTAKES << " mistakes)\n"
                      << ansi::RESET;
        } else {
            std::cout << ansi::GREEN << "  ✓ Correct!\n" << ansi::RESET;
            // clear pencil marks for this cell
            pencil_marks_[cell.first][cell.second].clear();
        }

        puzzle_.set(cell, val);
        undo_stack_.push({cell, prev, val});
    }

    void cmd_erase() {
        Coord cell = read_coord();
        if (is_clue(cell)) {
            std::cout << ansi::RED << "  Can't erase a given clue.\n" << ansi::RESET;
            return;
        }
        int prev = puzzle_.get(cell);
        puzzle_.set(cell, 0);
        undo_stack_.push({cell, prev, 0});
        pencil_marks_[cell.first][cell.second].clear();
        std::cout << "  Cell erased.\n";
    }

    void cmd_note() {
        Coord cell = read_coord();
        if (is_clue(cell)) {
            std::cout << ansi::RED << "  Can't add notes to a clue cell.\n" << ansi::RESET;
            return;
        }
        std::cout << "  Enter candidates to toggle (e.g. 1 3 7), then 0 to finish:\n  > ";
        auto& marks = pencil_marks_[cell.first][cell.second];
        int v;
        while (std::cin >> v && v != 0) {
            if (v < 1 || v > 9) continue;
            if (marks.count(v)) marks.erase(v);
            else                 marks.insert(v);
        }
        clear_cin();
        std::cout << "  Notes updated.\n";
    }

    void cmd_hint() {
        if (hints_used_ >= MAX_HINTS) {
            std::cout << ansi::YELLOW
                      << "  No more hints available.\n"
                      << ansi::RESET;
            return;
        }
        auto hint = give_hint(puzzle_, solution_);
        if (!hint) {
            std::cout << "  No hint available right now.\n";
            return;
        }
        ++hints_used_;
        Coord cell = hint->cell;
        int   val  = hint->value;

        std::cout << ansi::CYAN
                  << "  Hint " << hints_used_ << "/" << MAX_HINTS
                  << " → Row " << cell.first+1
                  << ", Col " << cell.second+1
                  << " = " << val << "\n"
                  << ansi::RESET;

        int prev = puzzle_.get(cell);
        puzzle_.set(cell, val);
        undo_stack_.push({cell, prev, val});
        pencil_marks_[cell.first][cell.second].clear();
    }

    void cmd_undo() {
        if (undo_stack_.empty()) {
            std::cout << "  Nothing to undo.\n";
            return;
        }
        Move m = undo_stack_.top();
        undo_stack_.pop();
        puzzle_.set(m.cell, m.prev_value);
        std::cout << "  Undone move at row "
                  << m.cell.first+1 << ", col " << m.cell.second+1 << ".\n";
    }

    void cmd_check() {
        bool ok = true;
        for (int r = 0; r < GRID_SIZE && ok; ++r)
            for (int c = 0; c < GRID_SIZE && ok; ++c) {
                Coord cell{r,c};
                int v = puzzle_.get(cell);
                if (v != 0 && solution_.get(cell) != v) ok = false;
            }
        if (ok)
            std::cout << ansi::GREEN << "  Board looks good so far!\n" << ansi::RESET;
        else
            std::cout << ansi::RED
                      << "  There are incorrect values on the board.\n"
                      << ansi::RESET;
    }

    void cmd_show_solution() {
        std::cout << ansi::YELLOW
                  << "  Show full solution? This will end the game. (y/n): "
                  << ansi::RESET;
        char c; std::cin >> c; clear_cin();
        if (c == 'y' || c == 'Y') {
            std::cout << "\nFull Solution:\n";
            print_board(solution_);
            game_over_ = true;
        }
    }

    void cmd_quit() {
        std::cout << "  Quitting game. Time: " << elapsed_str() << "\n";
        game_over_ = true;
    }

    //win/loss

    bool is_complete() const {
        if (!puzzle_loaded_) return false;
        return is_valid_solution(puzzle_);
    }

    void handle_win() {
        print_board(puzzle_);
        std::cout << ansi::GREEN << ansi::BOLD
                  << "\n🎉 Congratulations! Puzzle solved!\n"
                  << "   Time: " << elapsed_str()
                  << "  |  Mistakes: " << mistakes_
                  << "  |  Hints used: " << hints_used_ << "\n"
                  << ansi::RESET;
        game_over_ = true;
    }

    void handle_loss() {
        print_board(puzzle_);
        std::cout << ansi::RED << ansi::BOLD
                  << "\n💀 Game Over — too many mistakes ("
                  << MAX_MISTAKES << ").\n"
                  << ansi::RESET
                  << "\nThe solution was:\n";
        print_board(solution_);
        game_over_ = true;
    }
//display board
    void print_board(const Grid& g) const {
        std::cout << "\n";
        std::cout << "     1   2   3   4   5   6   7   8   9\n";
        std::cout << "   ╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n";

        for (int r = 0; r < GRID_SIZE; ++r) {
            std::cout << " " << r+1 << " ║";
            for (int c = 0; c < GRID_SIZE; ++c) {
                Coord cell{r,c};
                int v = g.get(cell);

                std::string cell_str;
                if (v == 0) {
                    auto& marks = pencil_marks_[r][c];
                    if (!marks.empty())
                        cell_str = ansi::DIM + "·" + ansi::RESET;
                    else
                        cell_str = " ";
                } else if (is_clue(cell)) {
                    cell_str = ansi::BOLD + std::to_string(v) + ansi::RESET;
                } else {
                    // player-placed: colour by correctness
                    if (solution_.get(cell) == v)
                        cell_str = ansi::CYAN + std::to_string(v) + ansi::RESET;
                    else
                        cell_str = ansi::RED + std::to_string(v) + ansi::RESET;
                }

                std::cout << " " << cell_str << " ";
                if (c == 2 || c == 5) std::cout << "║";
                else if (c < 8)       std::cout << "│";
            }
            std::cout << "║\n";

            if (r == 2 || r == 5) {
                std::cout << "   ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n";
            } else if (r < 8) {
                std::cout << "   ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n";
            }
        }
        std::cout << "   ╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n\n";
    }

    void print_status_bar() const {
        std::cout << ansi::BOLD
                  << "─────────────────────────────────────────\n"
                  << " ⏱  " << elapsed_str()
                  << "  │  ❤  " << (MAX_MISTAKES - mistakes_) << "/" << MAX_MISTAKES
                  << "  │  💡 Hints: " << (MAX_HINTS - hints_used_) << "/" << MAX_HINTS
                  << "\n─────────────────────────────────────────\n"
                  << ansi::RESET;
    }

    void print_controls() const {
        std::cout << ansi::DIM
                  << " p=place  e=erase  n=notes  h=hint  u=undo  c=check  s=solution  q=quit\n"
                  << ansi::RESET;
    }

    void print_banner() const {
        std::cout << ansi::BOLD << ansi::CYAN
                  << "\n"
                  << "  ███████╗██╗   ██╗██████╗  ██████╗ ██╗  ██╗██╗   ██╗\n"
                  << "  ██╔════╝██║   ██║██╔══██╗██╔═══██╗██║ ██╔╝██║   ██║\n"
                  << "  ███████╗██║   ██║██║  ██║██║   ██║█████╔╝ ██║   ██║\n"
                  << "  ╚════██║██║   ██║██║  ██║██║   ██║██╔═██╗ ██║   ██║\n"
                  << "  ███████║╚██████╔╝██████╔╝╚██████╔╝██║  ██╗╚██████╔╝\n"
                  << "  ╚══════╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝ ╚═════╝\n"
                  << "\n"
                  << ansi::RESET;
    }


    Difficulty choose_difficulty() {
        std::cout << "\nDifficulty:\n"
                  << "  1. Easy\n  2. Medium\n  3. Hard\n"
                  << "Choice: ";
        int ch = read_int(1, 3);
        switch (ch) {
            case 1: return Difficulty::EASY;
            case 3: return Difficulty::HARD;
            default: return Difficulty::MEDIUM;
        }
    }

    bool is_clue(Coord cell) const {
        return original_.get(cell) != 0;
    }

    std::string elapsed_str() const {
        if (!puzzle_loaded_) return "00:00";
        auto now  = Clock::now();
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(now - start_time_).count();
        int m = static_cast<int>(secs / 60);
        int s = static_cast<int>(secs % 60);
        char buf[16];
        std::snprintf(buf, sizeof(buf), "%02d:%02d", m, s);
        return buf;
    }

    void reset_state() {
        while (!undo_stack_.empty()) undo_stack_.pop();
        for (auto& row : pencil_marks_)
            for (auto& cell : row) cell.clear();
        hints_used_    = 0;
        mistakes_      = 0;
        game_over_     = false;
        puzzle_loaded_ = false;
    }

    // Input reading
    // Read an int in [lo, hi] — loops until valid
    static int read_int(int lo, int hi) {
        int v;
        while (true) {
            if (std::cin >> v && v >= lo && v <= hi) {
                clear_cin();
                return v;
            }
            clear_cin();
            std::cout << "  Please enter a number between "
                      << lo << " and " << hi << ": ";
        }
    }

    // Read row col as a Coord (1-indexed → 0-indexed internally)
    static Coord read_coord() {
        std::cout << "  Row (1-9): ";
        int r = read_int(1, 9) - 1;
        std::cout << "  Col (1-9): ";
        int c = read_int(1, 9) - 1;
        return {r, c};
    }

    // Read a single lowercase command char
    static std::string read_command() {
        std::string s;
        std::cin >> s;
        clear_cin();
        if (!s.empty()) s[0] = static_cast<char>(std::tolower(s[0]));
        return s;
    }

    static void clear_cin() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};
}