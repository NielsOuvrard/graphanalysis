/*
**    d888b  d8888b.  .d8b.  d8888b. db   db       .d8b.  d8b   db  .d8b.  db   db    db .d8888. d888888b .d8888.
**   88' Y8b 88  `8D d8' `8b 88  `8D 88   88      d8' `8b 888o  88 d8' `8b 88   `8b  d8' 88'  YP   `88'   88'  YP
**   88      88oobY' 88ooo88 88oodD' 88ooo88      88ooo88 88V8o 88 88ooo88 88    `8bd8'  `8bo.      88    `8bo.
**   88  ooo 88`8b   88~~~88 88~~~   88~~~88      88~~~88 88 V8o88 88~~~88 88      88      `Y8b.    88      `Y8b.
**   88. ~8~ 88 `88. 88   88 88      88   88      88   88 88  V888 88   88 88booo. 88    db   8D   .88.   db   8D
**    Y888P  88   YD YP   YP 88      YP   YP      YP   YP VP   V8P YP   YP Y88888P YP    `8888Y' Y888888P `8888Y'
**
**
*/

#include "file.hpp"

bool exists(std::vector<Person *> v, std::string s)
{
    for (auto person: v) {
        if (person->name == s)
            return true;
    }
    return false;
}

void floydWarshall(std::vector<std::vector<int>> &A)
{
    int n = A.size();

    // Initialize A(i, j) to infinity for pairs of vertices not directly connected
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && A[i][j] == 0) {
                A[i][j] = INT_MAX;
            }
        }
    }

    // Apply the Floyd-Warshall algorithm
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (A[i][k] != INT_MAX && A[k][j] != INT_MAX) {
                    A[i][j] = std::min(A[i][j], A[i][k] + A[k][j]);
                }
            }
        }
    }
}

std::vector<Person *> create_graph(std::vector<std::string> file)
{
    std::vector<Person *> friends;

    for (auto line: file) {
        std::string friend1 = line.substr(0, line.find(" is friends with "));
        std::string friend2 = line.substr(line.find(" is friends with ") + 17, line.length());

        if (!exists(friends, friend1)) {
            Person *p = new Person();
            p->name = friend1;
            friends.push_back(p);
        }
        if (!exists(friends, friend2)) {
            Person *p = new Person();
            p->name = friend2;
            friends.push_back(p);
        }
        Person *p1 = nullptr;
        Person *p2 = nullptr;

        for (auto &person: friends) {
            if (person->name == friend1)
                p1 = person;
            else if (person->name == friend2)
                p2 = person;
        }
        p1->friends.push_back(p2);
        p2->friends.push_back(p1);
    }
    return friends;
}

void print_graph(std::vector<Person *> friends)
{
    for (auto person: friends) {
        std::cout << person->name << std::endl;
        std::cout << "friends: ";
        for (auto friend_: person->friends) {
            std::cout << friend_->name << ", ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
}

//links - intermediate:
//    Passed: 100.0 %
//    Crashed: 0.0 %
//    optimization:
//    Passed: 50.0 %. # done probably
//                    Crashed: 50.0 %
//    optimization (eval): # done probably
//                         Passed: 0.0 %
//                         Crashed: 100.0 %
//                         plots - conspiracies:
//    Passed: 0.0 %
//    Crashed: 0.0 %
//    plots - parsing:
//    Passed: 25.0 %
//    Crashed: 75.0 %
//    plots - shortest paths matrix:
//    Passed: 20.0 %
//    Crashed: 80.0 %