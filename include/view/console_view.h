// Input, clear screen
#ifndef __CONSOLE_VIEW__
#define __CONSOLE_VIEW__

#include "include/model/board.h"
#include "include/model/player.h"
#include "include/model/matches_history.h"
using namespace std;

namespace CaroView
{
    class ConsoleView
    {
    public:
        void printCaroBoard();
        void displayMainMenu();
        void displayBotMenu();
        void displayGameResult(const string &resultMessage);
        void displayPlayerInfo(const Player &player);
        void displayAllPlayersInfo(const vector<Player *> &players);
        void displayMessage(const string &message);
        void displayMatchList(const vector<MatchRecord>& matches);
    };
} // namespace CaroView

#endif