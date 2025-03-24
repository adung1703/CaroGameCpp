#include "include/utils/utils.h"
#include <limits>
#include <iostream>

namespace Utils
{
    void printCaroBoard(const vector<vector<char>> &board, int size)
    {
        cout << YELLOW << "    ";
        for (int i = 0; i < size; ++i)
        {
            cout << i << "   ";
        }
        cout << RESET << endl;

        // Print the horizontal line
        cout << WHITE << "  ";
        for (int col = 0; col < size; ++col)
        {
            cout << "+---";
        }
        cout << "+" << RESET << endl;

        for (int row = 0; row < size; ++row)
        {
            cout << YELLOW << row << " " << RESET;
            for (int col = 0; col < size; ++col)
            {
                char cell = board[row][col];
                if (cell == 'X')
                    cout << "| " << RED << cell << " " << RESET;
                else if (cell == 'O')
                    cout << "| " << GREEN << cell << " " << RESET;
                else
                    cout << "|   ";
            }
            cout << "|" << endl;

            cout << WHITE << "  ";
            // Print the horizontal line
            for (int col = 0; col < size; ++col)
            {
                cout << "+---";
            }
            cout << "+" << RESET << endl;
        }
        cout << endl;
    }

    void displayMainMenu()
    {
        cout << BLUE << "*------ MAIN MENU ------*" << RESET << endl;
        cout << YELLOW << "Press number to choice:" << RESET << endl;
        cout << GREEN << "1. Play with Other Player" << RESET << endl;
        cout << GREEN << "2. Play with BOT" << RESET << endl;
        cout << GREEN << "3. Replay" << RESET << endl;
        cout << GREEN << "4. Player's Information" << RESET << endl;
        cout << GREEN << "5. Guild (Display All Players)" << RESET << endl;
        cout << RED << "6. Exit" << RESET << endl;
        cout << BLUE << "*----------------------*" << RESET << endl;
    }

    void displayBotMenu()
    {
        cout << BLUE << "*------ Play with BOT ------*" << RESET << endl;
        cout << YELLOW << "Press number to choice :" << RESET << endl;
        cout << GREEN << "1. Easy Mode" << RESET << endl;
        cout << YELLOW << "2. Normal Mode" << RESET << endl;
        cout << RED << "3. Hard Mode" << RESET << endl;
        cout << WHITE << "4. Back to MAIN MENU" << RESET << endl;
        cout << BLUE << "*---------------------------*" << RESET << endl;
    }
    void clearScreen()
    {
        CLEAR_SCREEN();
    }

    int getIntInput(const string &prompt)
    {
        int input;
        cout << YELLOW << prompt << RESET;
        while (!(cin >> input))
        {
            cout << RED << "Invalid input. Please enter an integer: " << RESET;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear extra input
        return input;
    }

    string getStringInput(const string &prompt)
    {
        string input;
        cout << YELLOW << prompt << RESET;
        getline(cin, input);
        return input;
    }
}
