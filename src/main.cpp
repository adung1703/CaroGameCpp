#include "include/controller/game_controller.h"

int main() {
    CaroController::GameController gameController; // Use GameController to run the game
    gameController.runGame(); // Start the game through the controller
    return 0;
}

// g++ src/main.cpp src/model/board.cpp src/model/player.cpp src/model/bot.cpp src/controller/game_controller.cpp src/utils/utils.cpp -o CaroGame 