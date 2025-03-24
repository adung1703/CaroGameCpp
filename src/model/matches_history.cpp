#include "include/model/matches_history.h"
#include "include/utils/utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;

MatchesHistory::MatchesHistory()
{
    loadHistory();
}

void MatchesHistory::loadHistory()
{
    matchHistory.clear();
    ifstream inFile(historyFile);
    if (!inFile.is_open())
    {
        cerr << "Error opening match history file: " << historyFile << endl;
        return; // Or throw exception
    }

    string line;
    while (getline(inFile, line))
    {
        istringstream iss(line);
        MatchRecord match;
        iss >> match.timestamp >> match.player1 >> match.player2;

        getline(inFile, line);
        istringstream moveStream(line);
        int row, col;
        while (moveStream >> row >> col)
        {
            match.moves.push_back(make_pair(row, col));
        }
        matchHistory.push_back(match);
    }
    inFile.close();
}

void MatchesHistory::saveMatch(const string &player1, const string &player2, const vector<pair<int, int>> &moves)
{
    ofstream outFile(historyFile, ios::app);
    if (!outFile.is_open())
        return;

    time_t now = time(0);
    tm tm = *localtime(&now);
    ostringstream oss;
    oss << put_time(&tm, "%Y/%m/%d_%H:%M:%S");
    string timestamp = oss.str();

    outFile << timestamp << " " << player1 << " " << player2 << endl;
    for (const pair<int, int> &move : moves)
    {
        outFile << move.first << " " << move.second << " ";
    }
    outFile << endl;
    outFile.close();
}

vector<MatchRecord> MatchesHistory::searchMatches(const string &player1, const string &player2)
{
    vector<MatchRecord> results;
    for (const MatchRecord &match : matchHistory)
    {
        if ((player1.empty() || match.player1 == player1 || match.player2 == player1) &&
            (player2.empty() || match.player1 == player2 || match.player2 == player2))
        {
            results.push_back(match);
        }
    }
    return results;
}
