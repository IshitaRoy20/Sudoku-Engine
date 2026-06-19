#include "src/game_engine.h"

int main() {
    try {
        sudoku::GameEngine::run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}