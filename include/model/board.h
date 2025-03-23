#ifndef __BOARD__
#define __BOARD__

// Singleton class for the game Board
#include <vector>
using namespace std;

class Board
{
private:
    vector<vector<char>> gameBoard;
    int boardSize;
    static Board *instance;
    Board(int size = 10);
    Board();
    bool checkLine(int row, int col, int dRow, int dCol, char playerSymbol) const;

public:
    ~Board(); 
    Board(const Board &) = delete;
    Board &operator=(const Board &) = delete; 
    static Board *getInstance(int size = 10);
    static void destroyInstance();

    void initBoard();
    void displayBoard() const;
    bool isValidMove(int row, int col) const;
    bool placeMove(int row, int col, char playerSymbol);
    bool checkThreat(int row, int col, char botSymbol) const;
    bool checkWin(int row, int col, char playerSymbol) const;
    bool checkAnyWin(char playerSymbol) const;
    bool isBoardFull() const;
    vector<vector<char>> getGameBoard();
    int getBoardSize() const;
    char getCell(int row, int col) const;
    void setCell(int row, int col, char symbol);
};

#endif