#ifndef TOURNAMENT_MANAGER_H
#define TOURNAMENT_MANAGER_H

#include "Leaderboard.h"
#include "BinomialHeap.h"
#include "Match.h"
#include "AppConfig.h"
#include "OperationLogger.h"

#include <unordered_map>
#include <string>

class TournamentManager {
private:
    Leaderboard leaderboard;
    BinomialHeap matchQueue;
    AppConfig config;
    OperationLogger logger;

    std::unordered_map<int, Match> matches;

    int nextMatchId;

public:
    explicit TournamentManager(const AppConfig& config = AppConfig());

    void run();

    void addPlayer(const std::string& name, int score);
    void updateScore(const std::string& name, int delta);
    void removePlayer(const std::string& name);

    void showTopPlayers(int k);
    void showLeaderboardStructure();

    void scheduleMatch(
        const std::string& player1,
        const std::string& player2,
        int priority
    );

    void nextMatch();

    void showMatchQueue();

    void printHelp() const;
};

#endif