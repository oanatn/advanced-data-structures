#include <iostream>
#include <sstream>
#include "RbTree.h"

int main() {
    RbTree t;
    std::cout<<"Please provide the input commands. Type EXIT to leave.\n";
    while (1) {
        std::string line;
        std::getline(std::cin,line);
        std::stringstream ss(line);
        std::string cmd;
        ss>>cmd;
        if(cmd=="ADD") {
            int x;
            ss>>x;
            t.insert(x);
        }
        else if(cmd=="REMOVE") {
            int x;
            ss>>x;
            t.deleteNode(x);
        }
        else if(cmd=="MEDIAN") {
            std::cout<<t.median()<<"\n";
        }
        else if (cmd=="EXIT") {
            std::cout<<"Goodbie!";
            break;
        }
        else {
            std::cout<<"Please provide a valid command: ADD, REMOVE, MEDIAN, or EXIT.\n";
        }
    }
    return 0;
}