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

using namespace std;

namespace CaroController
{

    GameController::GameController() : gameBoard(Board::getInstance()),
                                       player1(nullptr), player2(nullptr),
                                       currentPlayer(nullptr),
                                       gameRunning(false),
                                       consoleView(),
                                       playerManager(),
                                       matchesHistory()
    {
        playerManager.loadPlayers("../data/players.txt"); // Load player data at start
    }

    GameController::~GameController()
    {
        playerManager.savePlayers("../data/players.txt"); // Save player data on exit
        Board::destroyInstance();                         // Properly destroy Singleton instance
    }

    void GameController::runGame()
    {
        int choice;
        do
        {
            Utils::clearScreen();
            consoleView.displayMainMenu(); // Use View to display menu
            cout << YELLOW;                // Đổi màu thông báo nhập
            choice = Utils::getIntInput("Enter your choice: ");
            cout << RESET; // Reset lại màu sắc

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
                    cout << YELLOW;
                    botChoice = Utils::getIntInput("Enter Bot Difficulty: ");
                    cout << RESET;

                    if (botChoice >= 1 && botChoice <= 3)
                    {
                        startBotGame(botChoice);
                        break; // Back to main menu after bot game
                    }
                    else if (botChoice == 4)
                    {
                        break; // Back to main menu
                    }
                    else
                    {
                        cout << RED << "Invalid choice. Please try again." << RESET << endl;
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
            { // Exit
                cout << RED << "Exiting game. Goodbye!" << RESET << endl;
                break;
            }
            default:
                cout << RED << "Invalid choice. Please try again." << RESET << endl;
            }
        } while (choice != 6);
    }

    void GameController::initializeGame()
    {
        gameBoard->initBoard(); // Use pointer to access Singleton
        clearMoves();
        gameRunning = true;

        string player1Name = Utils::getStringInput("Enter Player 1 name (X): ");
        string player2Name = Utils::getStringInput("Enter Player 2 name (O): ");

        // Tìm người chơi có sẵn trong data
        player1 = playerManager.getPlayer(player1Name);
        if (player1 == nullptr)
        {
            player1 = new Player(player1Name, 'X');
            playerManager.addPlayer(player1);
        }
        else
        {
            player1->setSymbol('X'); // Set symbol for existing player
        }

        player2 = playerManager.getPlayer(player2Name);
        if (player2 == nullptr)
        {
            player2 = new Player(player2Name, 'O');
            playerManager.addPlayer(player2);
        }
        else
        {
            player2->setSymbol('O'); // Set symbol for existing player
        }

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
            // if (checkGameOver(-1, -1))
            //     break;
            switchPlayer();
        }
        displayGameResult();
    }

    void GameController::startBotGame(int botChoice)
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
        gameBoard->initBoard();
        clearMoves();
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
            // if (checkGameOver(-1, -1))
            //     break;
            switchPlayer();
        }
        displayGameResult();

        delete humanPlayer;
        delete botPlayer;
    }

    void GameController::handlePlayerTurn()
    {
        consoleView.displayMessage("Player " + string(1, currentPlayer->getSymbol()) + "'s turn (" + currentPlayer->getName() + "):");

        pair<int, int> move = currentPlayer->getMove();
        int row = move.first;
        int col = move.second;

        if (gameBoard->placeMove(row, col, currentPlayer->getSymbol()))
        { // Use pointer to access Singleton
            addMoves(row, col);
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

        // Check win
        if (row != -1 && col != -1 && gameBoard->checkWin(row, col, currentSymbol))
        {
            gameRunning = false;
            Utils::clearScreen();
            gameBoard->displayBoard(); // Show final board state
            cout << "Player " << currentSymbol << " (" << GREEN << currentPlayer->getName() << RESET << ") wins!" << endl;
            Player *opponent = (currentPlayer == player1) ? player2 : player1;
            currentPlayer->incrementWins();
            opponent->incrementLosses();
            matchesHistory.saveMatch(player1->getName(), player2->getName(), currentMatchMoves); // Save game history when a player wins
            playerManager.savePlayers("../data/players.txt");
            cout << YELLOW << "Game Over! \nPress Enter to continue..." << RESET;

            string input;
            getline(cin, input);
            return true;
        }
        // Check draw
        else if (gameBoard->isBoardFull())
        {
            gameRunning = false;
            Utils::clearScreen();
            gameBoard->displayBoard(); // Show final board state
            cout << BLUE << "It's a draw!" << RESET << endl;
            player1->incrementDraws();
            player2->incrementDraws();
            matchesHistory.saveMatch(player1->getName(), player2->getName(), currentMatchMoves); // Save game history when it's a draw
            playerManager.savePlayers("../data/players.txt");

            cout << YELLOW << "Game Over! \nPress Enter to continue..." << RESET;
            string input;
            getline(cin, input);
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
        cout << YELLOW << "Enter player 1 name (or leave blank to search all matches): " << RESET;
        string player1 = Utils::getStringInput("");
        cout << YELLOW << "Enter player 2 name (or leave blank): " << RESET;
        string player2 = Utils::getStringInput("");
    
        vector<MatchRecord> matches = matchesHistory.searchMatches(player1, player2);
        
        if (matches.empty())
        {
            cout << RED << "No match history found for the given players." << RESET << endl;
            return;
        }
    
        consoleView.displayMatchList(matches);
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
                cout << BLUE;
                consoleView.displayMessage("Recommended opponent of similar skill level: " + equivalentPlayer->getName());
                cout << RESET;
            }
        }
        else
        {
            cout << RED;
            consoleView.displayMessage("Player '" + playerName + "' not found.");
            cout << RESET;
        }
        cout << YELLOW;
        consoleView.displayMessage("Press Enter to continue...");
        cout << RESET;
        cin.get();
    }

    void GameController::showGuildInformation()
    {
        consoleView.displayAllPlayersInfo(playerManager.getAllPlayers()); // Use View to display all players
        consoleView.displayMessage("Press Enter to continue...");         // Use View
        cin.get();
    }

    // void GameController::undoLastMove()
    // {
    //     if (!currentMatchMoves.empty())
    //     {
    //         pair<int, int> lastMove = currentMatchMoves.back();
    //         currentMatchMoves.pop_back();
    //         gameBoard->setCell(lastMove.first, lastMove.second, ' ');
    //         switchPlayer(); // Switch back to previous player
    //         consoleView.displayMessage("Move undone.");
    //     }
    //     else
    //     {
    //         consoleView.displayMessage("No moves to undo.");
    //     }
    // }

} // namespace CaroController