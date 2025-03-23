#ifndef __PLAYER__
#define __PLAYER__

#include <string>
#include "board.h"
using namespace std;

class Player
{
protected:
    string playerName;
    char playerSymbol;
    int wins;
    int losses;
    int draws;

public:
    Player(const string &name, char symbol);
    virtual ~Player() = default;

    virtual pair<int, int> getMove();

    const string &getName() const;
    char getSymbol() const;
    void setSymbol(char symbol);
    int getWins() const;
    int getLosses() const;
    int getDraws() const;

    void incrementWins();
    void incrementLosses();
    void incrementDraws();
    void loadStats(int w, int l, int d);
    virtual void displayInfo() const;
};

#endif