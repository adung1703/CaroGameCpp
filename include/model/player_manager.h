// Model: Player data management
#ifndef __PLAYER_MANAGER__
#define __PLAYER_MANAGER__

#include <string>
#include <vector>
#include "player.h"

class PlayerManager
{
private:
    vector<Player *> players;
public:
    PlayerManager();
    ~PlayerManager();

    void loadPlayers(const string filename);
    void savePlayers(const string filename);
    void addPlayer(Player *player);
    void removePlayer(const string &name);
    Player *getPlayer(const string &name);
    vector<Player *> getAllPlayers() const;
    void displayPlayerInfo(const string &name);
    void displayAllPlayers() const;
    Player *findEquivalentLevelPlayer(const Player& player);
};

#endif