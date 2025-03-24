#include "include/model/player_manager.h"
#include "include/utils/utils.h"
#include <fstream>
#include <iostream>
#include <limits>

PlayerManager::PlayerManager() 
{
    players.resize(0); // Initialize players vector
}

PlayerManager::~PlayerManager()
{
    for (Player *player : players)
    {
        delete player;
    }
    players.clear();
}

void PlayerManager::loadPlayers(const string filename)
{
    ifstream inFile(filename);
    if (!inFile.is_open())
    {
        cerr << "Error opening player data file: " << filename << endl;
        return; // Or throw exception
    }

    string name;
    char symbol;
    int wins, losses, draws;
    
    while (inFile >> name >> symbol >> wins >> losses >> draws)
    {
        Player *player = new Player(name, symbol); // Assuming Player constructor is suitable
        player->loadStats(wins, losses, draws);
        players.push_back(player);
    }
    inFile.close();
}

void PlayerManager::savePlayers(const string filename)
{
    ofstream outFile(filename);
    if (!outFile.is_open())
    {
        cerr << "Error opening player data file for writing: " << filename << endl;
        return; // Or throw exception
    }

    for (Player *player : players)
    {
        outFile << player->getName() << " " << player->getSymbol() << " "
                << player->getWins() << " " << player->getLosses() << " " << player->getDraws() << endl;
    }
    outFile.close();
}

void PlayerManager::addPlayer(Player *player)
{
    players.push_back(player);
}

void PlayerManager::removePlayer(const string &name)
{
    for (vector<Player *>::iterator it = players.begin(); it != players.end(); ++it)
    {
        if ((*it)->getName() == name)
        {
            delete *it; 
            players.erase(it);
            return;
        }
    }
}

Player *PlayerManager::getPlayer(const string &name)
{
    for (Player *player : players)
    {
        if (player->getName() == name)
        {
            return player;
        }
    }
    return nullptr; // Player not found
}

vector<Player *> PlayerManager::getAllPlayers() const
{
    return players;
}

void PlayerManager::displayPlayerInfo(const string &name)
{
    Player *player = getPlayer(name);
    if (player)
    {
        player->displayInfo();
    }
    else
    {
        cout << "Player '" << name << "' not found." << endl;
    }
}

void PlayerManager::displayAllPlayers() const
{
    if (players.empty())
    {
        cout << "No players in the database." << endl;
        return;
    }
    for (const Player *player : players)
    {
        player->displayInfo();
        cout << "-----------------------" << endl;
    }
}

double calculateWinLossRatio(const Player &player)
{
    int losses = player.getLosses();
    int wins = player.getWins();
    if (losses == 0)
    {
        return (wins == 0) ? 0.0 : numeric_limits<double>::infinity(); // Avoid division by zero, handle no losses case
    }
    return static_cast<double>(wins) / losses;
}

Player *PlayerManager::findEquivalentLevelPlayer(const Player &player)
{
    double targetRatio = calculateWinLossRatio(player);
    Player *equivalentPlayer = nullptr;
    double minRatioDifference = numeric_limits<double>::max(); // Initialize with a large value

    for (Player *otherPlayer : players)
    {
        if (otherPlayer->getName() == player.getName())
        { // Skip comparing to the same player
            continue;
        }

        double otherRatio = calculateWinLossRatio(*otherPlayer);
        double ratioDifference = abs(targetRatio - otherRatio);

        if (ratioDifference < minRatioDifference)
        {
            minRatioDifference = ratioDifference;
            equivalentPlayer = otherPlayer; // Found a closer player
        }
    }

    if (equivalentPlayer)
    {
        cout << "Found equivalent level player: " << equivalentPlayer->getName()
                  << " (Win/Loss Ratio: " << calculateWinLossRatio(*equivalentPlayer) << ")" << endl;
    }
    else
    {
        cout << "No equivalent level player found." << endl;
    }
    return equivalentPlayer; // Will be nullptr if no player is considered equivalent
}