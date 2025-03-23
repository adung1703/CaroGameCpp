#ifndef __UTILS__
#define __UTILS__

#include <vector>
#include <string>
using namespace std;

namespace Utils
{
    void printCaroBoard(const vector<vector<char>> &board, int size);
    void displayMainMenu();
    void displayBotMenu();
    void clearScreen();
    int getIntInput(const string &prompt);
    string getStringInput(const string &prompt);
}

#endif