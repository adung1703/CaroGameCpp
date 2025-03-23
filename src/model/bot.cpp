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

int Bot::evaluateBoard(char botSymbol) const {
    Board &board = *Board::getInstance();
    char playerSymbol = (botSymbol == 'X') ? 'O' : 'X';
    int score = 0;

    vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (int row = 0; row < board.getBoardSize(); row++) {
        for (int col = 0; col < board.getBoardSize(); col++) {
            if (board.getCell(row, col) == botSymbol) {
                for (auto [dRow, dCol] : directions) {
                    int count = 1;
                    int i = 1;

                    while (i < 4) {
                        int newRow = row + i * dRow;
                        int newCol = col + i * dCol;
                        if (newRow >= 0 && newRow < board.getBoardSize() &&
                            newCol >= 0 && newCol < board.getBoardSize() &&
                            board.getCell(newRow, newCol) == botSymbol) {
                            count++;
                        } else {
                            break;
                        }
                        i++;
                    }

                    if (count == 4) score += 1000; // Thắng
                    else if (count == 3) score += 100;
                    else if (count == 2) score += 10;
                }
            } else if (board.getCell(row, col) == playerSymbol) {
                for (auto [dRow, dCol] : directions) {
                    int count = 1;
                    int i = 1;

                    while (i < 4) {
                        int newRow = row + i * dRow;
                        int newCol = col + i * dCol;
                        if (newRow >= 0 && newRow < board.getBoardSize() &&
                            newCol >= 0 && newCol < board.getBoardSize() &&
                            board.getCell(newRow, newCol) == playerSymbol) {
                            count++;
                        } else {
                            break;
                        }
                        i++;
                    }

                    if (count == 4) score -= 1000; // Đối thủ thắng
                    else if (count == 3) score -= 100;
                    else if (count == 2) score -= 10;
                }
            }
        }
    }

    return score;
}

int Bot::minimax(int depth, bool isMaximizing, int alpha, int beta) const {
    Board *board = Board::getInstance();
    
    if (depth == 0 || board->isBoardFull()) {
        return evaluateBoard(this->getSymbol());
    }

    char botSymbol = this->getSymbol();
    char playerSymbol = (botSymbol == 'X') ? 'O' : 'X';

    if (isMaximizing) { // Bot's turn
        int maxEval = -10000;
        for (int row = 0; row < board->getBoardSize(); row++) {
            for (int col = 0; col < board->getBoardSize(); col++) {
                if (board->isValidMove(row, col)) {
                    board->placeMove(row, col, botSymbol);
                    int eval = minimax(depth - 1, false, alpha, beta);
                    board->setCell(row, col, ' '); // Undo move
                    maxEval = max(maxEval, eval);
                    alpha = max(alpha, eval);
                    if (beta <= alpha) break; // Alpha-beta pruning
                }
            }
        }
        return maxEval;
    } else { // Opponent's turn
        int minEval = 10000;
        for (int row = 0; row < board->getBoardSize(); row++) {
            for (int col = 0; col < board->getBoardSize(); col++) {
                if (board->isValidMove(row, col)) {
                    board->placeMove(row, col, playerSymbol);
                    int eval = minimax(depth - 1, true, alpha, beta);
                    board->setCell(row, col, ' '); // Undo move
                    minEval = min(minEval, eval);
                    beta = min(beta, eval);
                    if (beta <= alpha) break;
                }
            }
        }
        return minEval;
    }
}

pair<int, int> Bot::getHardMove() const {
    Board &board = *Board::getInstance();
    int bestScore = -10000;
    pair<int, int> bestMove = {-1, -1};

    for (int row = 0; row < board.getBoardSize(); row++) {
        for (int col = 0; col < board.getBoardSize(); col++) {
            if (board.isValidMove(row, col)) {
                board.placeMove(row, col, this->getSymbol());
                int score = minimax(4, false, -10000, 10000);
                board.setCell(row, col, ' '); // Undo move

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {row, col};
                }
            }
        }
    }

    return bestMove;
}
