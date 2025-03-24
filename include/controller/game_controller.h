#ifndef __GAME_CONTROLLER__
#define __GAME_CONTROLLER__

#include "model/board.h"
#include "model/player.h"
#include "model/bot.h"
#include "model/player_manager.h"
#include "model/matches_history.h"
#include "view/console_view.h"
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
    vector<pair<int, int>> currentMatchMoves; 
    CaroView::ConsoleView consoleView; // Controller uses the View
    PlayerManager playerManager; // Controller manages PlayerManager Model
    MatchesHistory matchesHistory; // Controller manages MatchesHistory Model

public:
    GameController();
    ~GameController();

    void runGame(); // Main game loop controller
    void initializeGame();
    void startGame(); // Player vs Player game
    void startBotGame(int botChoice);
    void handlePlayerTurn();
    void switchPlayer();
    bool checkGameOver(int lastMoveRow, int lastMoveCol);
    void displayGameResult();
    void replayGame();
    void showPlayerInformation();
    void showGuildInformation();
    // void undoLastMove();

private:
    void clearMoves() { currentMatchMoves.clear(); }
    void addMoves(int row, int col) { currentMatchMoves.push_back({row, col}); }
};

} // namespace CaroController

#endif