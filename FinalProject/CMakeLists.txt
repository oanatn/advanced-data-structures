cmake_minimum_required(VERSION 3.30)
project(FinalProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(FinalProject main.cpp
        Leaderboard.cpp
        Leaderboard.h
        RbTree.cpp
        RbTree.h
        BinomialHeap.cpp
        BinomialHeap.h
        Match.h
        TournamentManager.cpp
        TournamentManager.h
        AppConfig.h
        OperationLogger.h
        OperationLogger.cpp)
