#include "include/view/console_view.h"
#include "include/utils/utils.h"
#include <iostream>

namespace CaroView {

Board& board = *Board::getInstance();

void ConsoleView::printCaroBoard() {
    Utils::printCaroBoard(board.getGameBoard(), board.getBoardSize());
}

void ConsoleView::displayMainMenu() {
    Utils::displayMainMenu();
}

void ConsoleView::displayBotMenu() {
    Utils::displayBotMenu();
}

void ConsoleView::displayGameResult(const string& resultMessage) {
    Utils::clearScreen();
    cout << "Final Board:" << endl;
    printCaroBoard();
    cout << resultMessage << endl;
    cout << "-----------------------" << endl;
}

void ConsoleView::displayPlayerInfo(const Player& player) {
    player.displayInfo();
}

void ConsoleView::displayAllPlayersInfo(const vector<Player*>& players) {
    if (players.empty()) {
        cout << "No players in the database." << endl;
        return;
    }
    for (const Player* player : players) {
        displayPlayerInfo(*player);
        cout << "-----------------------" << endl;
    }
}

void ConsoleView::displayMessage(const string& message) {
    cout << message << endl;
}


} // namespace CaroView