#ifndef __UTILS__
#define __UTILS__

#ifdef _WIN32
    #define CLEAR_SCREEN() system("cls") // Windows
#else
    #define CLEAR_SCREEN() system("clear") // Linux, macOS
#endif

#include <vector>
#include <string>
using namespace std;

namespace Utils
{
    #define RESET "\033[0m"
    #define RED "\033[31m"
    #define GREEN "\033[32m"
    #define YELLOW "\033[33m"
    #define BLUE "\033[34m"
    #define WHITE "\033[37m"
    void printCaroBoard(const vector<vector<char>> &board, int size);
    void displayMainMenu();
    void displayBotMenu();
    void clearScreen();
    int getIntInput(const string &prompt);
    string getStringInput(const string &prompt);
}

#endif