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

#include <queue>

pair<int, int> Bot::getNormalMove() const
{
    Board &board = *Board::getInstance();
    char botSymbol = this->getSymbol();
    char playerSymbol = (botSymbol == 'X') ? 'O' : 'X';

    // Priority queue để lưu trữ nước đi theo độ ưu tiên (max-heap)
    priority_queue<pair<int, pair<int, int>>> moveQueue;

    for (int row = 0; row < board.getBoardSize(); ++row)
    {
        for (int col = 0; col < board.getBoardSize(); ++col)
        {
            if (!board.isValidMove(row, col)) continue;

            int priority = 0;

            // 1️. Nếu bot có thể thắng ngay, đánh ngay (+1000 điểm)
            board.placeMove(row, col, botSymbol);
            if (board.checkWin(row, col, botSymbol))
                return {row, col};  // Trả về ngay
            board.setCell(row, col, ' ');

            // 2️. Nếu đối thủ có thể thắng ngay, chặn ngay (+900 điểm)
            board.placeMove(row, col, playerSymbol);
            if (board.checkWin(row, col, playerSymbol))
                priority = max(priority, 900);
            board.setCell(row, col, ' ');

            // 3️. Nếu đối thủ có 2 nước liên tiếp mở, chặn ngay (+500 điểm)
            if (board.checkThreat(row, col, botSymbol))
                priority = max(priority, 500);

            // 4️. Nếu bot có 2 nước liên tiếp mở, đánh tiếp (+400 điểm)
            if (board.checkThreat(row, col, playerSymbol))
                priority = max(priority, 400);

            // 5️. Nếu không có nước đi tối ưu, không đưa vào hàng đợi
            if (priority > 0) moveQueue.push({priority, {row, col}});
        }
    }

    // Chọn nước đi có độ ưu tiên cao nhất
    if (!moveQueue.empty()) return moveQueue.top().second;

    // Nếu không tìm được nước đi phù hợp, chơi như Easy Mode
    return getEasyMove();
}

int Bot::evaluateBoard(char botSymbol) const {
    Board &board = *Board::getInstance();
    char playerSymbol = (botSymbol == 'X') ? 'O' : 'X';
    int score = 0;

    vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (int row = 0; row < board.getBoardSize(); row++) {
        for (int col = 0; col < board.getBoardSize(); col++) {
            // Evaluate score for Bot symbol
            if (board.getCell(row, col) == botSymbol) {
                for (pair<int, int> d : directions) {
                    int dRow = d.first;
                    int dCol = d.second;
                    int bot_count = 1;
                    int player_block = 0; // Count opponent's blocks at the ends of streaks
                    int empty_space = 0;  // Count empty spaces at the ends of streaks

                    for (int i = 1; i < 4; ++i) {
                        int newRow = row + i * dRow;
                        int newCol = col + i * dCol;
                        if (newRow >= 0 && newRow < board.getBoardSize() &&
                            newCol >= 0 && newCol < board.getBoardSize()) {
                            if (board.getCell(newRow, newCol) == botSymbol) {
                                bot_count++;
                            } else if (board.getCell(newRow, newCol) == playerSymbol) {
                                player_block++;
                                break; // Blocked by player, stop counting in this direction
                            } else {
                                empty_space++;
                            }
                        } else {
                            break; // Out of board
                        }
                    }

                    if (bot_count == 4) score += 100000; // Immediate win is highest priority
                    else if (bot_count == 3 && empty_space > 0) score += 5000; // Potential win, high priority
                    else if (bot_count == 2 && empty_space > 1) score += 500; // Good to build
                    else if (bot_count == 1 && empty_space > 2) score += 50; // Minor advantage

                    if (bot_count >= 2) { // Check for blockable threats as well
                        int block_count = bot_count;
                        int block_player_block = player_block;
                        int block_empty_space = empty_space;
                        // Check for blockable threats in reverse direction
                        for (int i = -1; i > -4; --i) {
                            int newRow = row + i * dRow;
                            int newCol = col + i * dCol;
                            if (newRow >= 0 && newRow < board.getBoardSize() &&
                                newCol >= 0 && newCol < board.getBoardSize()) {
                                if (board.getCell(newRow, newCol) == botSymbol) {
                                    block_count++;
                                } else if (board.getCell(newRow, newCol) == playerSymbol) {
                                    block_player_block++;
                                    break;
                                } else {
                                    block_empty_space++;
                                }
                            } else {
                                break;
                            }
                        }
                        if (block_count == 4) score += 100000; // Immediate win is highest priority
                        else if (block_count == 3 && block_empty_space > 0) score += 5000; // Potential win, high priority
                        else if (block_count == 2 && block_empty_space > 1) score += 500; // Good to build
                        else if (block_count == 1 && block_empty_space > 2) score += 50; // Minor advantage
                    }
                }
            }
            // Evaluate score for Player symbol (penalize opponent streaks)
            else if (board.getCell(row, col) == playerSymbol) {
                for (pair<int, int> d : directions) {
                    int dRow = d.first;
                    int dCol = d.second;
                    int player_count = 1;
                    int bot_block = 0;
                    int empty_space = 0;

                    for (int i = 1; i < 4; ++i) {
                        int newRow = row + i * dRow;
                        int newCol = col + i * dCol;
                        if (newRow >= 0 && newRow < board.getBoardSize() &&
                            newCol >= 0 && newCol < board.getBoardSize()) {
                            if (board.getCell(newRow, newCol) == playerSymbol) {
                                player_count++;
                            } else if (board.getCell(newRow, newCol) == botSymbol) {
                                bot_block++;
                                break;
                            } else {
                                empty_space++;
                            }
                        } else {
                            break;
                        }
                    }
                    if (player_count == 4) score -= 100000; // Player win is highest negative penalty
                    else if (player_count == 3 && empty_space > 0) score -= 5000; // Block potential player win
                    else if (player_count == 2 && empty_space > 1) score -= 500; // Block player build
                    else if (player_count == 1 && empty_space > 2) score -= 50; // Minor disadvantage

                    if (player_count >= 2) {
                        int block_count = player_count;
                        int block_bot_block = bot_block;
                        int block_empty_space = empty_space;

                        for (int i = -1; i > -4; --i) {
                            int newRow = row + i * dRow;
                            int newCol = col + i * dCol;
                            if (newRow >= 0 && newRow < board.getBoardSize() &&
                                newCol >= 0 && newCol < board.getBoardSize()) {
                                if (board.getCell(newRow, newCol) == playerSymbol) {
                                    block_count++;
                                } else if (board.getCell(newRow, newCol) == botSymbol) {
                                    block_bot_block++;
                                    break;
                                } else {
                                    block_empty_space++;
                                }
                            } else {
                                break;
                            }
                        }
                         if (block_count == 4) score -= 100000; // Player win is highest negative penalty
                        else if (block_count == 3 && block_empty_space > 0) score -= 5000; // Block potential player win
                        else if (block_count == 2 && block_empty_space > 1) score -= 500; // Block player build
                        else if (block_count == 1 && block_empty_space > 2) score -= 50; // Minor disadvantage
                    }
                }
            }
        }
    }

    return score;
}


int Bot::minimax(int depth, bool isMaximizing, int alpha, int beta) const {
    Board &board = *Board::getInstance();

    if (depth == 0 || board.isBoardFull() || board.checkAnyWin(this->getSymbol()) || board.checkAnyWin((this->getSymbol() == 'X' ? 'O' : 'X'))) { // More comprehensive base cases
        return evaluateBoard(this->getSymbol());
    }

    char botSymbol = this->getSymbol();
    char playerSymbol = (botSymbol == 'X') ? 'O' : 'X';

    if (isMaximizing) { // Bot's turn (Maximizing player)
        int maxEval = -numeric_limits<int>::max(); // Initialize to lowest possible value
        for (int row = 0; row < board.getBoardSize(); row++) {
            for (int col = 0; col < board.getBoardSize(); col++) {
                if (board.isValidMove(row, col)) {
                    board.placeMove(row, col, botSymbol);
                    int eval = minimax(depth - 1, false, alpha, beta); // Recurse for opponent's turn (minimizing)
                    board.setCell(row, col, ' '); // Undo move - crucial for backtracking
                    maxEval = max(maxEval, eval); // Bot wants to maximize its score
                    alpha = max(alpha, eval);      // Update alpha for pruning
                    if (beta <= alpha) break;       // Alpha-beta pruning: prune if beta is <= alpha
                }
            }
        }
        return maxEval; // Return the maximum evaluation found
    } else { // Player's turn (Minimizing player)
        int minEval = numeric_limits<int>::max(); // Initialize to highest possible value
        for (int row = 0; row < board.getBoardSize(); row++) {
            for (int col = 0; col < board.getBoardSize(); col++) {
                if (board.isValidMove(row, col)) {
                    board.placeMove(row, col, playerSymbol);
                    int eval = minimax(depth - 1, true, alpha, beta); // Recurse for Bot's turn (maximizing)
                    board.setCell(row, col, ' '); // Undo move - crucial for backtracking
                    minEval = min(minEval, eval); // Player wants to minimize Bot's score (maximize player's score which is negative for Bot's perspective)
                    beta = min(beta, eval);       // Update beta for pruning
                    if (beta <= alpha) break;       // Alpha-beta pruning: prune if beta is <= alpha
                }
            }
        }
        return minEval; // Return the minimum evaluation found
    }
}


pair<int, int> Bot::getHardMove() const {
    Board &board = *Board::getInstance();
    int bestScore = -numeric_limits<int>::max();
    pair<int, int> bestMove = {-1, -1};
    int alpha = -numeric_limits<int>::max(); // Initialize alpha for root minimax call
    int beta = numeric_limits<int>::max();  // Initialize beta for root minimax call
    char botSymbol = this->getSymbol();

    for (int row = 0; row < board.getBoardSize(); row++) {
        for (int col = 0; col < board.getBoardSize(); col++) {
            if (board.isValidMove(row, col)) {
                board.placeMove(row, col, botSymbol);
                int score = minimax(3, false, alpha, beta); // Depth reduced to 3 for performance, start with false for opponent's turn after bot moves
                board.setCell(row, col, ' '); // Undo move

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {row, col};
                }
                 alpha = max(alpha, bestScore); // Update alpha at root level
            }
        }
    }

    return bestMove;
}
