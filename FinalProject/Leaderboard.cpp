#include "Leaderboard.h"
#include <iostream>

void Leaderboard::setLogger(OperationLogger* logger) {
    tree.setLogger(logger);
}

bool Leaderboard::contains(const std::string& player) const {
    return scores.find(player) != scores.end();
}

bool Leaderboard::add(const std::string& player, int score) {
    if (contains(player)) {
        std::cout << "Player already exists: " << player << "\n";
        return false;
    }

    scores[player] = score;
    tree.insert(score, player);

    std::cout << "Added player " << player << " with score " << score << ".\n";
    return true;
}

bool Leaderboard::update(const std::string& player, int delta) {
    auto it = scores.find(player);

    if (it == scores.end()) {
        std::cout << "Player not found: " << player << "\n";
        return false;
    }

    int oldScore = it->second;
    int newScore = oldScore + delta;

    tree.deleteNode(oldScore, player);
    scores[player] = newScore;
    tree.insert(newScore, player);

    std::cout << "Updated " << player << ": " << oldScore << " -> " << newScore << ".\n";
    return true;
}

bool Leaderboard::remove(const std::string& player) {
    auto it = scores.find(player);

    if (it == scores.end()) {
        std::cout << "Player not found: " << player << "\n";
        return false;
    }

    int score = it->second;
    tree.deleteNode(score, player);
    scores.erase(it);

    std::cout << "Removed player " << player << ".\n";
    return true;
}

void Leaderboard::top(int k) const {
    if (k <= 0) {
        std::cout << "Please enter a positive number.\n";
        return;
    }

    tree.top(k);
}

void Leaderboard::showStructure() const {
    tree.showTree();
}