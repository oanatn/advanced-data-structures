#include"Leaderboard.h"

void Leaderboard::add(std::string player, int score) {
    scores[player]=score;
    tree.insert(score, player);
}

void Leaderboard::update(std::string player, int delta) {
    int oldScore=scores[player];
    tree.deleteNode(oldScore, player);
    int newScore=oldScore+delta;
    scores[player]=newScore;
    tree.insert(newScore, player);
}

void Leaderboard::remove(std::string player) {
    int score=scores[player];
    tree.deleteNode(score, player);
    scores.erase(player);
}

void Leaderboard::top(int k) {
    tree.top(k);
}

