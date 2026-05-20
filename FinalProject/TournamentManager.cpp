#include "TournamentManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

TournamentManager::TournamentManager(const AppConfig& config)
    : config(config), logger(config.verbose), nextMatchId(1) {

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    leaderboard.setLogger(&logger);
    matchQueue.setLogger(&logger);

    if (this->config.verbose) {
        std::cout << "Verbose mode enabled.\n";
    }
}

void TournamentManager::addPlayer(const std::string& name, int score) {
    logger.clear();
    logger.log("Command: add a new player to the leaderboard.");
    logger.log("The leaderboard stores players inside a red-black tree ordered by score.");

    leaderboard.add(name, score);

    logger.print();
}

void TournamentManager::updateScore(const std::string& name, int delta) {
    logger.clear();
    logger.log("Command: update a player's score.");
    logger.log("The old score is removed from the red-black tree, then the new score is inserted.");

    leaderboard.update(name, delta);

    logger.print();
}

void TournamentManager::removePlayer(const std::string& name) {
    logger.clear();
    logger.log("Command: remove a player from the leaderboard.");
    logger.log("Removing a player deletes their score node from the red-black tree.");

    leaderboard.remove(name);

    logger.print();
}

void TournamentManager::showTopPlayers(int k) {
    logger.clear();
    logger.log("Command: show top players.");
    logger.log("The red-black tree is traversed from highest score to lowest score.");

    leaderboard.top(k);

    logger.print();
}

void TournamentManager::showLeaderboardStructure() {
    logger.clear();
    logger.log("Command: show leaderboard structure.");
    logger.log("The red-black tree is printed sideways: right subtree, root, left subtree.");

    leaderboard.showStructure();

    logger.print();
}

void TournamentManager::scheduleMatch(
    const std::string& player1,
    const std::string& player2,
    int priority
) {
    logger.clear();

    logger.log("Command: schedule a match.");
    logger.log("Checking whether both players exist in the leaderboard.");

    if (!leaderboard.contains(player1)) {
        std::cout << "Player not found: " << player1 << "\n";
        logger.print();
        return;
    }

    if (!leaderboard.contains(player2)) {
        std::cout << "Player not found: " << player2 << "\n";
        logger.print();
        return;
    }

    logger.log("Both players exist.");

    if (player1 == player2) {
        std::cout << "A player cannot be matched against themselves.\n";
        logger.print();
        return;
    }

    if (priority <= 0) {
        std::cout << "Priority must be a positive integer. Lower number means higher priority.\n";
        logger.print();
        return;
    }

    logger.log("Priority is valid.");
    logger.log("Creating match object.");
    logger.log("Inserting match priority into the binomial heap.");

    Match match(nextMatchId, player1, player2, priority);

    matches.emplace(nextMatchId, match);

    matchQueue.insert(priority, nextMatchId);

    std::cout
        << "Scheduled match #" << nextMatchId
        << ": "
        << player1
        << " vs "
        << player2
        << " with priority "
        << priority
        << ".\n";

    logger.log("Match was stored in the pending match table.");
    logger.log("The binomial heap now controls which match is played next.");

    logger.print();

    nextMatchId++;
}

void TournamentManager::nextMatch() {
    logger.clear();

    logger.log("Command: play the next scheduled match.");
    logger.log("The binomial heap extracts the match with the smallest priority value.");

    HeapNode* node = matchQueue.extractMin();

    if (node == nullptr) {
        std::cout << "No matches scheduled.\n";
        logger.print();
        return;
    }

    int matchId = node->matchId;

    auto it = matches.find(matchId);

    if (it != matches.end()) {
        const Match& match = it->second;

        std::cout
            << "Next match: "
            << match.player1
            << " vs "
            << match.player2
            << " (priority "
            << match.priority
            << ")\n";

        bool firstPlayerWins = std::rand() % 2 == 0;
        std::string winner = firstPlayerWins ? match.player1 : match.player2;

        std::cout << "Winner selected randomly: " << winner << "\n";
        std::cout << "Awarding 300 points to " << winner << ".\n";

        logger.log("The winner receives 300 points.");
        logger.log("Updating the leaderboard will delete the old score and insert the new score.");

        leaderboard.update(winner, 300);

        matches.erase(it);

        logger.log("The completed match was removed from the pending match table.");
    } else {
        std::cout << "Error: match data not found for match #" << matchId << ".\n";
        logger.log("The heap returned a match id that was not found in the match table.");
    }

    delete node;

    logger.print();
}

void TournamentManager::showMatchQueue() {
    logger.clear();

    logger.log("Command: show match queue.");
    logger.log("The binomial heap is printed as a forest of binomial trees.");

    matchQueue.print();

    logger.print();
}

bool TournamentManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file) {
        std::cout << "Could not open file for saving: " << filename << "\n";
        return false;
    }

    std::vector<std::pair<std::string, int>> players = leaderboard.getPlayers();

    file << "PLAYERS " << players.size() << "\n";

    for (const auto& player : players) {
        file << player.first << " " << player.second << "\n";
    }

    file << "MATCHES " << matches.size() << "\n";

    for (const auto& entry : matches) {
        const Match& match = entry.second;

        file
            << match.id << " "
            << match.player1 << " "
            << match.player2 << " "
            << match.priority << "\n";
    }

    file << "NEXT_MATCH_ID " << nextMatchId << "\n";

    std::cout << "Saved tournament to " << filename << ".\n";
    return true;
}

bool TournamentManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        std::cout << "Could not open file for loading: " << filename << "\n";
        return false;
    }

    std::string section;
    int playerCount;

    if (!(file >> section >> playerCount) || section != "PLAYERS" || playerCount < 0) {
        std::cout << "Invalid file format: expected PLAYERS section.\n";
        return false;
    }

    std::vector<std::pair<std::string, int>> loadedPlayers;

    for (int i = 0; i < playerCount; i++) {
        std::string name;
        int score;

        if (!(file >> name >> score)) {
            std::cout << "Invalid file format while reading players.\n";
            return false;
        }

        loadedPlayers.emplace_back(name, score);
    }

    int matchCount;

    if (!(file >> section >> matchCount) || section != "MATCHES" || matchCount < 0) {
        std::cout << "Invalid file format: expected MATCHES section.\n";
        return false;
    }

    std::vector<Match> loadedMatches;

    for (int i = 0; i < matchCount; i++) {
        int id;
        std::string player1;
        std::string player2;
        int priority;

        if (!(file >> id >> player1 >> player2 >> priority)) {
            std::cout << "Invalid file format while reading matches.\n";
            return false;
        }

        loadedMatches.emplace_back(id, player1, player2, priority);
    }

    int loadedNextMatchId;

    if (!(file >> section >> loadedNextMatchId) || section != "NEXT_MATCH_ID" || loadedNextMatchId <= 0) {
        std::cout << "Invalid file format: expected NEXT_MATCH_ID section.\n";
        return false;
    }

    std::vector<std::pair<std::string, int>> currentPlayers = leaderboard.getPlayers();

    for (const auto& player : currentPlayers) {
        leaderboard.remove(player.first);
    }

    matchQueue.clear();
    matches.clear();

    nextMatchId = loadedNextMatchId;

    for (const auto& player : loadedPlayers) {
        leaderboard.add(player.first, player.second);
    }

    for (const Match& match : loadedMatches) {
        if (!leaderboard.contains(match.player1) || !leaderboard.contains(match.player2)) {
            std::cout
                << "Skipping match #"
                << match.id
                << " because one or both players do not exist.\n";
            continue;
        }

        if (match.priority <= 0) {
            std::cout
                << "Skipping match #"
                << match.id
                << " because priority is invalid.\n";
            continue;
        }

        matches.emplace(match.id, match);
        matchQueue.insert(match.priority, match.id);
    }

    std::cout << "Loaded tournament from " << filename << ".\n";
    return true;
}

void TournamentManager::printHelp() const {
    std::cout << "\nAvailable commands:\n\n";

    std::cout << "add_player <name> <score>\n";
    std::cout << "update_score <name> <delta>\n";
    std::cout << "remove_player <name>\n";

    std::cout << "top <k>\n";

    std::cout << "schedule_match <player1> <player2> <priority>\n";
    std::cout << "next_match\n";

    std::cout << "show_leaderboard_structure\n";
    std::cout << "show_match_queue\n";

    std::cout << "save <file>\n";
    std::cout << "load <file>\n";

    std::cout << "help\n";
    std::cout << "exit\n\n";
}

void TournamentManager::run() {
    std::string line;

    printHelp();

    while (true) {
        std::cout << "contest> ";

        std::getline(std::cin, line);

        std::stringstream ss(line);

        std::string command;
        ss >> command;

        if (command == "add_player") {
            std::string name;
            int score;

            if (!(ss >> name >> score)) {
                std::cout << "Usage: add_player <name> <score>\n";
                continue;
            }

            addPlayer(name, score);
        }
        else if (command == "update_score") {
            std::string name;
            int delta;

            if (!(ss >> name >> delta)) {
                std::cout << "Usage: update_score <name> <delta>\n";
                continue;
            }

            updateScore(name, delta);
        }
        else if (command == "remove_player") {
            std::string name;

            if (!(ss >> name)) {
                std::cout << "Usage: remove_player <name>\n";
                continue;
            }

            removePlayer(name);
        }
        else if (command == "top") {
            int k;

            if (!(ss >> k)) {
                std::cout << "Usage: top <k>\n";
                continue;
            }

            showTopPlayers(k);
        }
        else if (command == "schedule_match") {
            std::string p1;
            std::string p2;
            int priority;

            if (!(ss >> p1 >> p2 >> priority)) {
                std::cout << "Usage: schedule_match <player1> <player2> <priority>\n";
                continue;
            }

            scheduleMatch(p1, p2, priority);
        }
        else if (command == "next_match") {
            nextMatch();
        }
        else if (command == "show_leaderboard_structure") {
            showLeaderboardStructure();
        }
        else if (command == "show_match_queue") {
            showMatchQueue();
        }
        else if (command == "save") {
            std::string filename;

            if (!(ss >> filename)) {
                std::cout << "Usage: save <file>\n";
                continue;
            }

            saveToFile(filename);
        }
        else if (command == "load") {
            std::string filename;

            if (!(ss >> filename)) {
                std::cout << "Usage: load <file>\n";
                continue;
            }

            loadFromFile(filename);
        }
        else if (command == "help") {
            printHelp();
        }
        else if (command == "exit") {
            std::cout << "Goodbye.\n";
            break;
        }
        else if (!command.empty()) {
            std::cout << "Unknown command. Type 'help' to see the list of available commands.\n";
        }
    }
}