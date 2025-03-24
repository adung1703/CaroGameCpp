#ifndef __MATCHES_HISTORY__
#define __MATCHES_HISTORY__

#include <vector>
#include <string>
using namespace std;

struct MatchRecord {
    string player1;
    string player2;
    string timestamp;
    vector<pair<int, int>> moves;
};

class MatchesHistory {
private:
    vector<MatchRecord> matchHistory;
    string historyFile = "../data/game_history.txt";

public:
    MatchesHistory();
    
    void loadHistory();
    void saveMatch(const string& player1, const string& player2, const vector<pair<int, int>>& moves);
    
    vector<MatchRecord> searchMatches(const string& player1, const string& player2);
};

#endif


