#include "TournamentManager.h"
#include "AppConfig.h"
#include <iostream>
#include <string>

void printStartupHelp() {
    std::cout << "Usage:\n";
    std::cout << "  FinalProject [options]\n\n";

    std::cout << "Options:\n";
    std::cout << "  -v, --verbose    Enable verbose mode\n";
    std::cout << "  -h, --help       Show this help message\n";
}

bool parseArguments(int argc, char* argv[], AppConfig& config) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-v" || arg == "--verbose") {
            config.verbose = true;
        }
        else if (arg == "-h" || arg == "--help") {
            printStartupHelp();
            return false;
        }
        else {
            std::cout << "Unknown option: " << arg << "\n";
            std::cout << "Use --help to see available options.\n";
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    AppConfig config;

    bool shouldRun = parseArguments(argc, argv, config);

    if (!shouldRun) {
        return 0;
    }

    TournamentManager manager(config);
    manager.run();

    return 0;
}