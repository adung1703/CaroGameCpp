#include "include/controller/game_controller.h"
#include "include/utils/utils.h"
#include "include/model/board.h"
#include "include/model/player.h"
#include "include/model/bot.h"
#include "include/view/console_view.h"
#include "include/model/player_manager.h"
#include <iostream>
#include <fstream>
#include <limits>

namespace CaroController
{

    GameController::GameController() : gameBoard(Board::getInstance()), player1(nullptr), player2(nullptr), currentPlayer(nullptr), gameRunning(false), consoleView(), playerManager()
    {
        playerManager.loadPlayers("data/players.txt"); // Load player data at start
    }

    GameController::~GameController()
    {
        delete player1;
        delete player2;
        Board::destroyInstance();                      // Properly destroy Singleton instance
        playerManager.savePlayers("data/players.txt"); // Save player data on exit
    }

    void GameController::runGame()
    {
        int choice;
        do
        {
            Utils::clearScreen();
            consoleView.displayMainMenu(); // Use View to display menu
            choice = Utils::getIntInput("Enter your choice: ");

            switch (choice)
            {
            case 1:
            { // Play with Other Player
                startGame();
                break;
            }
            case 2:
            { // Play with BOT
                int botChoice;
                do
                {
                    Utils::clearScreen();
                    consoleView.displayBotMenu(); // Use View to display bot menu
                    botChoice = Utils::getIntInput("Enter Bot Difficulty: ");
                    if (botChoice >= 1 && botChoice <= 3)
                    {
                        Difficulty botDifficulty;
                        if (botChoice == 1)
                            botDifficulty = Difficulty::EASY;
                        else if (botChoice == 2)
                            botDifficulty = Difficulty::NORMAL;
                        else
                            botDifficulty = Difficulty::HARD;

                        string playerName = Utils::getStringInput("Enter your name (X): ");
                        Player *humanPlayer = new Player(playerName, 'X');
                        Bot *botPlayer = new Bot("CaroBot", 'O', botDifficulty);

                        startBotGame(humanPlayer, botPlayer);

                        delete humanPlayer;
                        delete botPlayer;
                        break; // Back to main menu after bot game
                    }
                    else if (botChoice == 4)
                    {
                        break; // Back to main menu
                    }
                    else
                    {
                        consoleView.displayMessage("Invalid choice. Please try again."); // Use View to display message
                    }
                } while (true);
                break;
            }
            case 3:
            { // Replay
                replayGame();
                break;
            }
            case 4:
            { // Player's Information
                showPlayerInformation();
                break;
            }
            case 5:
            { // Guild (Display All Players)
                showGuildInformation();
                break;
            }
            case 6:
            {                                                         // Exit
                consoleView.displayMessage("Exiting game. Goodbye!"); // Use View to display message
                break;
            }
            default:
            {
                consoleView.displayMessage("Invalid choice. Please try again."); // Use View to display message
            }
            }
        } while (choice != 6);
    }

    void GameController::initializeGame()
    {
        gameBoard->initBoard(); // Use pointer to access Singleton
        clearMoveHistory();
        gameRunning = true;

        string player1Name = Utils::getStringInput("Enter Player 1 name (X): ");
        string player2Name = Utils::getStringInput("Enter Player 2 name (O): ");

        player1 = new Player(player1Name, 'X');
        player2 = new Player(player2Name, 'O');
        currentPlayer = player1; // Player 1 starts
    }

    void GameController::startGame()
    {
        initializeGame();
        while (gameRunning)
        {
            Utils::clearScreen();
            consoleView.displayMessage("=> Player 1 = X (" + player1->getName() + "), Player 2 = O (" + player2->getName() + "):"); // Use View
            consoleView.printCaroBoard();                                                                                           // Use View to print board
            handlePlayerTurn();
            if (checkGameOver(-1, -1))
                break;
            switchPlayer();
        }
        displayGameResult();
    }

    void GameController::startBotGame(Player *humanPlayer, Bot *botPlayer)
    {
        gameBoard->initBoard();
        clearMoveHistory();
        gameRunning = true;

        player1 = humanPlayer;
        player2 = botPlayer;
        currentPlayer = player1;

        while (gameRunning)
        {
            Utils::clearScreen();
            consoleView.displayMessage("=> Player 1 = X (" + player1->getName() + "), Player 2 = O (BOT - " + player2->getName() + "):");
            consoleView.printCaroBoard();
            handlePlayerTurn();
            if (checkGameOver(-1, -1))
                break;
            switchPlayer();
        }
        displayGameResult();
    }

    void GameController::handlePlayerTurn()
    {
        consoleView.displayMessage("Player " + string(1, currentPlayer->getSymbol()) + "'s turn (" + currentPlayer->getName() + "):");
        consoleView.displayMessage("Enter 'u' to undo last move or make a new move:");

        string input = Utils::getStringInput("Your choice: ");
        if (input == "u" || input == "U")
        {
            undoLastMove();
            return;
        }

        pair<int, int> move = currentPlayer->getMove();
        int row = move.first;
        int col = move.second;

        if (gameBoard->placeMove(row, col, currentPlayer->getSymbol()))
        { // Use pointer to access Singleton
            addToMoveHistory(row, col);
            if (checkGameOver(row, col))
                return;
        }
        else
        {
            consoleView.displayMessage("Invalid move. Please try again."); // Use View
        }
    }

    void GameController::switchPlayer()
    {
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }

    bool GameController::checkGameOver(int lastMoveRow, int lastMoveCol)
    {
        char currentSymbol = currentPlayer->getSymbol();
        int row = lastMoveRow;
        int col = lastMoveCol;

        if (row != -1 && col != -1 && gameBoard->checkWin(row, col, currentSymbol))
        { // Use pointer to access Singleton
            gameRunning = false;
            consoleView.displayMessage("Player " + string(1, currentSymbol) + " (" + currentPlayer->getName() + ") wins!"); // Use View
            currentPlayer->incrementWins();
            (currentPlayer == player1 ? player2 : player1)->incrementLosses();
            return true;
        }
        else if (gameBoard->isBoardFull())
        { // Use pointer to access Singleton
            gameRunning = false;
            consoleView.displayMessage("It's a draw!"); // Use View
            player1->incrementDraws();
            player2->incrementDraws();
            return true;
        }
        return false;
    }

    void GameController::displayGameResult()
    {
        consoleView.displayGameResult("Game Over!"); // Use View to display result
    }

    void GameController::replayGame()
    {
        if (moveHistory.empty())
        {
            consoleView.displayMessage("No game history to replay."); // Use View
            return;
        }

        gameBoard->initBoard(); // Use pointer to access Singleton
        Utils::clearScreen();
        consoleView.displayMessage("Replaying Game:"); // Use View
        consoleView.printCaroBoard();                  // Use View to print replay board
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        char p1Symbol = player1->getSymbol();
        char p2Symbol = player2->getSymbol();
        Player *replayCurrentPlayer = player1;

        for (const auto &move : moveHistory)
        {
            int row = move.first;
            int col = move.second;
            char symbol = replayCurrentPlayer->getSymbol();

            gameBoard->placeMove(row, col, symbol);
            Utils::clearScreen();
            consoleView.displayMessage("Replaying Game - Move by Player " + string(1, symbol) + " (" + replayCurrentPlayer->getName() + "):"); // Use View
            consoleView.printCaroBoard();                                                                                                      // Use View to print replay board
            consoleView.displayMessage("Press Enter to see next move...");                                                                     // Use View
            cin.get();

            replayCurrentPlayer = (replayCurrentPlayer == player1) ? player2 : player1;
        }
        consoleView.displayMessage("End of Replay."); // Use View
        displayGameResult();
    }

    void GameController::saveGameHistory()
    {
        std::ofstream outFile("data/game_history.txt"); // Open file to save game history
        if (outFile.is_open())
        {
            for (const auto &move : moveHistory)
            {
                outFile << move.first << " " << move.second << std::endl; // Save each move (row col)
            }
            outFile.close();
            consoleView.displayMessage("Game history saved to data/game_history.txt");
        }
        else
        {
            consoleView.displayMessage("Error saving game history!");
        }
    }

    void GameController::loadGameHistory()
    {
        moveHistory.clear();                           // Clear any existing history before loading
        std::ifstream inFile("data/game_history.txt"); // Open file to load game history
        if (inFile.is_open())
        {
            int row, col;
            int movesLoaded = 0;
            while (inFile >> row >> col)
            {
                moveHistory.push_back({row, col}); // Load moves from file
                movesLoaded++;
            }
            inFile.close();
            consoleView.displayMessage("Loaded " + std::to_string(movesLoaded) + " moves from data/game_history.txt");
            if (moveHistory.empty())
            {
                consoleView.displayMessage("data/game_history.txt is empty. No game to replay.");
            }
        }
        else
        {
            consoleView.displayMessage("No game history file found. Starting new replay.");
        }
    }

    // In GameController.cpp, add to showPlayerInformation method:
    void GameController::showPlayerInformation()
    {
        string playerName = Utils::getStringInput("Enter player name to search: ");
        Player *player = playerManager.getPlayer(playerName);
        if (player)
        {
            consoleView.displayPlayerInfo(*player);

            // Add this section to recommend equivalent player
            Player *equivalentPlayer = playerManager.findEquivalentLevelPlayer(*player);
            if (equivalentPlayer)
            {
                consoleView.displayMessage("Recommended opponent of similar skill level: " + equivalentPlayer->getName());
            }
        }
        else
        {
            consoleView.displayMessage("Player '" + playerName + "' not found.");
        }
        consoleView.displayMessage("Press Enter to continue...");
        cin.get();
    }
    
    void GameController::showGuildInformation()
    {
        consoleView.displayAllPlayersInfo(playerManager.getAllPlayers()); // Use View to display all players
        consoleView.displayMessage("Press Enter to continue...");         // Use View
        cin.get();
    }

    void GameController::undoLastMove()
    {
        if (!moveHistory.empty())
        {
            auto lastMove = moveHistory.back();
            moveHistory.pop_back();
            gameBoard->setCell(lastMove.first, lastMove.second, ' ');
            switchPlayer(); // Switch back to previous player
            consoleView.displayMessage("Move undone.");
        }
        else
        {
            consoleView.displayMessage("No moves to undo.");
        }
    }

} // namespace CaroController