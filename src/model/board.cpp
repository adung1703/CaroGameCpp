#include "include/model/board.h"
#include "include/utils/utils.h"

#include <iostream>
using namespace std;

Board *Board::instance = nullptr;

Board::Board() {};

Board::Board(int size) : boardSize(size), gameBoard(size, vector<char>(size, ' ')) {}

Board::~Board()
{
    instance = nullptr;
}

Board *Board::getInstance(int size)
{
    if (instance == nullptr)
    {
        instance = new Board(size);
    }
    return instance;
}

void Board::destroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

void Board::initBoard()
{
    gameBoard.assign(boardSize, vector<char>(boardSize, ' '));
}

void Board::displayBoard() const
{
    Utils::printCaroBoard(gameBoard, boardSize);
}

bool Board::isValidMove(int row, int col) const
{
    return (row >= 0 && row < boardSize && col >= 0 && col < boardSize && gameBoard[row][col] == ' ');
}

bool Board::placeMove(int row, int col, char playerSymbol)
{
    if (isValidMove(row, col))
    {
        gameBoard[row][col] = playerSymbol;
        return true;
    }
    return false;
}

// {dRow, dCol} = {1, 0} for vertical, {0, 1} for horizontal, {1, 1} for diagonal, {1, -1} for diagonal /
bool Board::checkLine(int row, int col, int dRow, int dCol, char playerSymbol) const
{
    int count = 0;

    // Check up to 3 positions in both directions
    for (int i = -3; i <= 3; ++i)
    {
        int checkRow = row + i * dRow;
        int checkCol = col + i * dCol;

        if (checkRow >= 0 && checkRow < boardSize &&
            checkCol >= 0 && checkCol < boardSize)
        {
            if (gameBoard[checkRow][checkCol] == playerSymbol)
            {
                count++;
                if (count >= 4) return true; // 4 in a row found
            }
            else
            {
                count = 0; // Reset count if sequence is broken
            }
        }
    }
    return false;
}

/*
Hàm kiểm tra xem bên cạnh vị trí hiện tại
có 2 ô liên tiếp nước của đối phương và bot chưa chặn đầu nào
Ví dụ mô phỏng: (Bot Symbol là O, Player Symbol là X)
| | | | |
| |X| | |
| | |X| |
| | | | |
Bot sẽ đánh tiếp vào (0, 0) hoặc (3, 3)
*/
bool Board::checkThreat(int row, int col, char mySymbol) const
{
    if (getCell(row, col) != ' ')
    {
        return false;
    }

    char competitorSymbol = (mySymbol == 'X') ? 'O' : 'X';

    pair<int, int> directions[] = {
        {1, 0}, // Vertical
        {0, 1}, // Horizontal
        {1, 1}, // Diagonal
        {1, -1} // Diagonal
    };

    for (const auto &dir : directions)
    {
        int dRow = dir.first;
        int dCol = dir.second;
        // Xét chiều âm
        int count = 0;
        int otherHeadRow = row - 3 * dRow;
        int otherHeadCol = col - 3 * dCol;
        char otherHeadSymbol = mySymbol;
        if (otherHeadRow >= 0 && otherHeadRow < getBoardSize() &&
            otherHeadCol >= 0 && otherHeadCol < getBoardSize())
        {
            otherHeadSymbol = getCell(otherHeadRow, otherHeadCol);
        }

        if (otherHeadSymbol == ' ')
        {
            for (int i = -2; i < 0; i++)
            {
                int checkRow = row + i * dRow;
                int checkCol = col + i * dCol;
                if (checkRow >= 0 && checkRow < getBoardSize() &&
                    checkCol >= 0 && checkCol < getBoardSize() &&
                    getCell(checkRow, checkCol) == competitorSymbol)
                {
                    count++;
                }
            }
        }
        if (count >= 2)
        {
            return true;
        }

        // Xét chiều dương
        count = 0;
        otherHeadRow = row + 3 * dRow;
        otherHeadCol = col + 3 * dCol;
        otherHeadSymbol = mySymbol;
        if (otherHeadRow >= 0 && otherHeadRow < getBoardSize() &&
            otherHeadCol >= 0 && otherHeadCol < getBoardSize())
        {
            otherHeadSymbol = getCell(otherHeadRow, otherHeadCol);
        }

        if (otherHeadSymbol == ' ')
        {
            for (int i = 1; i <= 2; i++)
            {
                int checkRow = row + i * dRow;
                int checkCol = col + i * dCol;
                if (checkRow >= 0 && checkRow < getBoardSize() &&
                    checkCol >= 0 && checkCol < getBoardSize() &&
                    getCell(checkRow, checkCol) == competitorSymbol)
                {
                    count++;
                }
            }

            if (count >= 2)
            {
                return true;
            }
        }
    }
    return false;
}

bool Board::checkWin(int row, int col, char playerSymbol) const
{
    // Check all four directions: horizontal, vertical, diagonal
    return checkLine(row, col, 1, 0, playerSymbol) || // Vertical
           checkLine(row, col, 0, 1, playerSymbol) || // Horizontal
           checkLine(row, col, 1, 1, playerSymbol) || // Diagonal
           checkLine(row, col, 1, -1, playerSymbol);  // Diagonal
}

bool Board::checkAnyWin(char playerSymbol) const
{
    for (int row = 0; row < boardSize; ++row)
    {
        for (int col = 0; col < boardSize; ++col)
        {
            if (gameBoard[row][col] == playerSymbol)
            {
                if (checkWin(row, col, playerSymbol))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::isBoardFull() const
{
    for (int row = 0; row < boardSize; ++row)
    {
        for (int col = 0; col < boardSize; ++col)
        {
            if (gameBoard[row][col] == ' ')
            {
                return false;
            }
        }
    }
    return true;
}

vector<vector<char>> Board::getGameBoard()
{
    return gameBoard;
}

int Board::getBoardSize() const
{
    return boardSize;
};

char Board::getCell(int row, int col) const
{
    if (row >= 0 && row < boardSize &&
        col >= 0 && col < boardSize)
    {
        return gameBoard[row][col];
    }
    return ' ';
}

void Board::setCell(int row, int col, char symbol)
{
    if (row >= 0 && row < boardSize &&
        col >= 0 && col < boardSize)
    {
        gameBoard[row][col] = symbol;
    }
}

void Board::replayMatch(const MatchRecord &match)
{
    initBoard();

    for (int i = 0; i < match.moves.size(); ++i)
    {
        cout << "Replaying Match: " << match.timestamp << " | "
        << match.player1 << " vs " << match.player2 << endl;
        Utils::clearScreen();
        pair<int, int> move = match.moves[i];
        placeMove(move.first, move.second, (i % 2 == 0) ? 'X' : 'O');
        displayBoard();
        cout << "Press Enter for next move...";
        cin.get();
    }

    cout << "End of Replay." << endl;
}