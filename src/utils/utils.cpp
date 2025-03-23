#include "include/utils/utils.h"
#include <limits>
#include <iostream>

namespace Utils
{
    void printCaroBoard(const vector<vector<char>> &board, int size)
    {
        cout << "    ";
        for (int i = 0; i < size; ++i)
        {
            cout << i << "   ";
        }
        cout << endl;

        // Print the horizontal line
        cout << "  ";
        for (int col = 0; col < size; ++col)
        {

            cout << "+---";
        }
        cout << "+" << endl;
        for (int row = 0; row < size; ++row)
        {
            cout << row << " ";
            for (int col = 0; col < size; ++col)
            {
                cout << "| " << board[row][col] << " ";
            }
            cout << "|" << endl;

            cout << "  ";
            // Print the horizontal line
            for (int col = 0; col < size; ++col)
            {

                cout << "+---";
            }
            cout << "+" << endl;
        }
        cout << endl;
    }

    void displayMainMenu()
    {
        cout << "* ------MAIN MENU-------*" << endl;
        cout << "Press number to choice:" << endl;
        cout << "1. Play with Other Player" << endl;
        cout << "2. Play with BOT" << endl;
        cout << "3. Replay" << endl;
        cout << "4. Player's Information" << endl;
        cout << "5. Guide" << endl;
        cout << "6. Exit" << endl;
        cout << "*-----------------------*" << endl;
    }

    void displayBotMenu()
    {
        cout << "*------Play with BOT-------*" << endl;
        cout << "Press number to choice :" << endl;
        cout << "1. Easy Mode" << endl;
        cout << "2. Normal Mode" << endl;
        cout << "3. Hard Mode" << endl;
        cout << "4. Back to MAIN MENU" << endl;
        cout << "*--------------------------*" << endl;
    }

    void clearScreen()
    {
        system("cls");
    }

    int getIntInput(const string &prompt)
    {
        int input;
        cout << prompt;
        while (!(cin >> input))
        {
            cout << "Invalid input. Please enter an integer: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear extra input
        return input;
    }
    
    string getStringInput(const string &prompt)
    {
        string input;
        cout << prompt;
        getline(cin, input);
        return input;
    }
}
