#ifndef MATCH_H
#define MATCH_H

#include <string>

struct Match {
    int id;
    std::string player1;
    std::string player2;
    int priority;

    Match(int id, const std::string& player1, const std::string& player2, int priority)
        : id(id), player1(player1), player2(player2), priority(priority) {}
};

#endif