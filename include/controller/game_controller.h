#ifndef __GAME_CONTROLLER__
#define __GAME_CONTROLLER__

#include "model/board.h"
#include "model/player.h"
#include "model/bot.h"
#include "view/console_view.h"
#include "model/player_manager.h"
#include <vector>
#include <string>

namespace CaroController {

class GameController {
private:
    Board* gameBoard; // Controller manages the Board Model (Singleton)
    Player* player1;
    Player* player2;
    Player* currentPlayer;
    bool gameRunning;
    vector<pair<int, int>> moveHistory;
    CaroView::ConsoleView consoleView; // Controller uses the View
    PlayerManager playerManager; // Controller manages PlayerManager Model

public:
    GameController();
    ~GameController();

    void runGame(); // Main game loop controller
    void initializeGame();
    void startGame(); // Player vs Player game
    void startBotGame(Player* humanPlayer, Bot* botPlayer);
    void handlePlayerTurn();
    void switchPlayer();
    bool checkGameOver(int lastMoveRow, int lastMoveCol);
    void displayGameResult();
    void replayGame();
    void saveGameHistory();
    void loadGameHistory();
    void showPlayerInformation();
    void showGuildInformation();

private:
    void clearMoveHistory() { moveHistory.clear(); }
    void addToMoveHistory(int row, int col) { moveHistory.push_back({row, col}); }
};

} // namespace CaroController

#endif