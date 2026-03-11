cmake_minimum_required(VERSION 3.30)
project(lab3_scoreboard)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab3_scoreboard main.cpp
        Leaderboard.h
        RbTree.cpp
        Leaderboard.cpp)
