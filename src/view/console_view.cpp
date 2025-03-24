#include "include/view/console_view.h"
#include "include/utils/utils.h"
#include <iostream>
#include <iomanip>

namespace CaroView
{

    Board &board = *Board::getInstance();

    void ConsoleView::printCaroBoard()
    {
        Utils::printCaroBoard(board.getGameBoard(), board.getBoardSize());
    }

    void ConsoleView::displayMainMenu()
    {
        Utils::displayMainMenu();
    }

    void ConsoleView::displayBotMenu()
    {
        Utils::displayBotMenu();
    }

    void ConsoleView::displayGameResult(const string &resultMessage)
    {
        Utils::clearScreen();
        cout << "Final Board:" << endl;
        printCaroBoard();
        cout << resultMessage << endl;
        cout << "-----------------------" << endl;
    }

    void ConsoleView::displayPlayerInfo(const Player &player)
    {
        player.displayInfo();
    }

    void ConsoleView::displayAllPlayersInfo(const vector<Player *> &players)
    {
        if (players.empty())
        {
            cout << "No players in the database." << endl;
            return;
        }
        for (const Player *player : players)
        {
            displayPlayerInfo(*player);
            cout << "-----------------------" << endl;
        }
    }

    void ConsoleView::displayMessage(const string &message)
    {
        cout << message << endl;
    }

    void ConsoleView::displayMatchList(const vector<MatchRecord> &matches)
    {
        if (matches.empty())
        {
            cout << "No matches found." << endl;
            return;
        }

        const int pageSize = 10;
        int page = 0;
        while (page * pageSize < matches.size())
        {
            Utils::clearScreen();
            cout << "----- Match History ----- (Page " << (page + 1) << ")\n";
            for (int i = 0; i < pageSize && page * pageSize + i < matches.size(); ++i)
            {
                int index = page * pageSize + i;
                cout << index + 1 << ". " << matches[index].timestamp << " | "
                     << matches[index].player1 << " vs " << matches[index].player2
                     << " (" << matches[index].moves.size() << " moves)" << endl;
            }

            cout << "Enter match number to replay, press Enter for next page, or 0 to exit: ";
            string input;
            getline(cin, input);
            if (input.empty())
            {
                ++page;
            }
            else
            {
                int choice = stoi(input);
                if (choice == 0)
                    break;
                if (choice > 0 && choice <= matches.size())
                {
                    board.replayMatch(matches[choice - 1]);
                }
            }
        }
    }

} // namespace CaroView