#include <iostream>
#include <sstream>
#include "Leaderboard.h"

int main() {
    Leaderboard l;
    std::cout<<"Please provide the input commands. Type EXIT to leave.\n";
    while (1) {
        std::string line;
        std::getline(std::cin,line);
        std::stringstream ss(line);
        std::string cmd;
        ss>>cmd;
        if(cmd=="ADD") {
            std::string name;
            int score;
            ss>>name>>score;
            l.add(name,score);
        }
        else if(cmd=="UPDATE") {
            std::string name;
            int delta;
            ss>>name>>delta;
            l.update(name,delta);
        }
        else if(cmd=="REMOVE") {
            std::string name;
            ss>>name;
            l.remove(name);
        }
        else if(cmd=="TOP") {
            int k;
            ss>>k;
            l.top(k);
        }
        else if (cmd=="EXIT") {
            std::cout<<"Goodbie!";
            break;
        }
        else {
            std::cout<<"Please provide a valid command: ADD, UPDATE, TOP, REMOVE, or EXIT.\n";
        }
    }
    return 0;
}