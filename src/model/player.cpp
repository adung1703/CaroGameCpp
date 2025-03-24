#include "include/model/player.h"
#include "include/utils/utils.h"
#include <iostream>
using namespace std;

Player::Player(const string &name, char playerSymbol, int wins, int losses, int draws) :
    playerName(name),
    playerSymbol(playerSymbol),
    wins(wins),
    losses(losses),
    draws(draws)
{}

pair<int, int> Player::getMove()
{
    Board &board = *Board::getInstance();
    int row, col;
    while (true)
    {
        row = Utils::getIntInput("Enter row (0-" + to_string(board.getBoardSize() - 1) + "): ");
        col = Utils::getIntInput("Enter column (0-" + to_string(board.getBoardSize() - 1) + "): ");
        if (board.isValidMove(row, col))
        {
            break;
        }
        else
        {
            cout << "Invalid move. Try again." << endl;
        }
    }
    return make_pair(row, col);
}

const string &Player::getName() const 
{ 
    return playerName; 
}

char Player::getSymbol() const 
{ 
    return playerSymbol; 
}
void Player::setSymbol(char symbol) 
{ 
    playerSymbol = symbol; 
}
int Player::getWins() const 
{ 
    return wins; 
}
int Player::getLosses() const 
{ 
    return losses; 
}
int Player::getDraws() const 
{ 
    return draws; 
}

void Player::incrementWins() 
{ 
    ++wins; 
}
void Player::incrementLosses() 
{ 
    ++losses; 
}
void Player::incrementDraws() 
{ 
    ++draws; 
}

void Player::loadStats(int w, int l, int d)
{
    wins = w;
    losses = l;
    draws = d;
}

void Player::displayInfo() const
{
    cout << "Player Name: " << playerName << endl;
    cout << "Wins: " << wins << endl;
    cout << "Losses: " << losses << endl;
    cout << "Draws: " << draws << endl;
}