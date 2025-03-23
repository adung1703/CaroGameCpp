#include "include/model/bot.h"
#include "include/model/board.h"
#include <cstdlib>
#include <ctime>
#include <limits>
#include <vector>

Bot::Bot(const string &name, char symbol, Difficulty difficultyLevel) : Player(name, symbol),
                                                                        difficultyLevel(difficultyLevel)
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

pair<int, int> Bot::getMove()
{
    switch (difficultyLevel)
    {
    case Difficulty::EASY:
        return getEasyMove();
    case Difficulty::NORMAL:
        return getNormalMove();
    case Difficulty::HARD:
        return getHardMove();
    default:
        return getEasyMove();
    }
}

pair<int, int> Bot::getEasyMove() const
{
    Board &board = *Board::getInstance();
    vector<pair<int, int>> possibleMoves;
    for (int row = 0; row < board.getBoardSize(); ++row)
    {
        for (int col = 0; col < board.getBoardSize(); ++col)
        {
            if (board.isValidMove(row, col))
            {
                possibleMoves.push_back(make_pair(row, col));
            }
        }
    }
    if (!possibleMoves.empty())
    {
        int randomIndex = rand() % possibleMoves.size();
        return possibleMoves[randomIndex];
    }

    return make_pair(-1, -1);
}

pair<int, int> Bot::getNormalMove() const
{
    Board &board = *Board::getInstance();
    for (int row = 0; row < board.getBoardSize(); ++row)
    {
        for (int col = 0; col < board.getBoardSize(); ++col)
        {
            if (board.isValidMove(row, col))
            {
                // Nếu bot có thể thắng ngay
                board.placeMove(row, col, this->getSymbol());
                if (board.checkWin(row, col, this->getSymbol()))
                {
                    board.setCell(row, col, ' ');
                    return make_pair(row, col);
                }
                board.setCell(row, col, ' ');

                // Nếu đối thủ có thể thắng ở nước tiếp theo
                char playerSymbol = (this->getSymbol() == 'X') ? 'O' : 'X';
                board.placeMove(row, col, playerSymbol);
                if (board.checkWin(row, col, playerSymbol))
                {
                    board.setCell(row, col, ' ');
                    return make_pair(row, col);
                }
                board.setCell(row, col, ' ');

                // Nếu có 2 ô liên tiếp của đối thủ
                if (board.checkThreat(row, col, this->getSymbol()))
                {
                    return make_pair(row, col);
                }

                // Nếu có 2 ô liên tiếp của bot
                if (board.checkThreat(row, col, playerSymbol))
                {
                    return make_pair(row, col);
                }
            }
        }
    }
    return getEasyMove();
}

int evaluateBoard(char botSymbol)
{
    Board &board = *Board::getInstance();
    if (board.checkAnyWin(botSymbol)) // Check win for Bot
    {
        return 10;
    }
    char playerSymbol = (botSymbol == 'X') ? 'O' : 'X';
    if (board.checkAnyWin(playerSymbol)) // Check win for Player
    {
        return -10;
    }
    if (board.isBoardFull())
    {
        return 0; // Draw
    }
    return 0; // Neutral state for now
}

int minimax(int depth, bool isMaximizingPlayer, char botSymbol)
{
    Board &board = *Board::getInstance();
    int score = evaluateBoard(botSymbol);

    // Base cases
    if (score == 10 || score == -10 || score == 0)
    {
        return score;
    }

    if (isMaximizingPlayer)
    {
        int bestScore = -numeric_limits<int>::infinity();
        for (int row = 0; row < board.getBoardSize(); ++row)
        {
            for (int col = 0; col < board.getBoardSize(); ++col)
            {
                if (board.isValidMove(row, col))
                {
                    board.placeMove(row, col, botSymbol);
                    bestScore = max(bestScore, minimax(depth + 1, false, botSymbol));
                }
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = numeric_limits<int>::infinity();
        char playerSymbol = (botSymbol == 'X') ? 'O' : 'X';
        for (int row = 0; row < board.getBoardSize(); ++row)
        {
            for (int col = 0; col < board.getBoardSize(); ++col)
            {
                if (board.isValidMove(row, col))
                {
                    board.placeMove(row, col, playerSymbol);
                    bestScore = min(bestScore, minimax(depth + 1, true, botSymbol));
                }
            }
        }
        return bestScore;
    }
}

pair<int, int> Bot::getHardMove() const
{
    Board &board = *Board::getInstance();
    int bestScore = -numeric_limits<int>::infinity();
    pair<int, int> bestMove = make_pair(-1, -1);
    char botSymbol = this->getSymbol();

    for (int row = 0; row < board.getBoardSize(); ++row)
    {
        for (int col = 0; col < board.getBoardSize(); ++col)
        {
            if (board.isValidMove(row, col))
            {
                board.placeMove(row, col, botSymbol);
                int moveScore = minimax(0, false, botSymbol); // Start minimax from player's perspective
                board.setCell(row, col, ' '); // Undo the move
                if (moveScore > bestScore)
                {
                    bestScore = moveScore;
                    bestMove = make_pair(row, col);
                }
            }
        }
    }
    return bestMove;
}