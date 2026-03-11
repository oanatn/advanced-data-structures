#include "RbTree.h"
#include <unordered_map>
#include <string>

class Leaderboard {
private:
    RbTree tree;
    std::unordered_map<std::string,int> scores;

public:
    void add(std::string player, int score);
    void update(std::string player, int delta);
    void remove(std::string player);
    void top(int k);
};