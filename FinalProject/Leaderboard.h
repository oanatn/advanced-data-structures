#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "RbTree.h"
#include "OperationLogger.h"

#include <unordered_map>
#include <string>

class Leaderboard {
private:
    RbTree tree;
    std::unordered_map<std::string, int> scores;

public:
    void setLogger(OperationLogger* logger);

    bool contains(const std::string& player) const;
    bool add(const std::string& player, int score);
    bool update(const std::string& player, int delta);
    bool remove(const std::string& player);

    void top(int k) const;
    void showStructure() const;
};

#endif