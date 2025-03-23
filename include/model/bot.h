#ifndef __BOT__
#define __BOT__

#include "player.h"
#include "board.h"

enum Difficulty
{
    EASY,
    NORMAL,
    HARD
};

class Bot : public Player
{
private:
    Difficulty difficultyLevel;
    pair<int, int> getEasyMove() const;
    pair<int, int> getNormalMove() const;
    pair<int, int> getHardMove() const;
    int minimax(int depth, bool isMaximizing, int alpha, int beta) const;
    int evaluateBoard(char botSymbol) const;

public:
    Bot(const string &name, char symbol, Difficulty difficultyLevel = Difficulty::NORMAL);
    ~Bot() override = default;
    pair<int, int> getMove() override;
};

#endif