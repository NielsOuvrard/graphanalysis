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

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <vector>

class Person
{
public:
    std::string name;
    std::vector<Person *> friends;
    std::vector<Person *> plots;
};

// * tools

Person *exists_person(std::vector<Person *> v, std::string s);

std::vector<Person *> create_graph(std::string filename);

void print_graph(std::vector<Person *> friends);

std::vector<std::string> file_to_vector(std::string filename);


void floydWarshall(std::vector<std::vector<int>> &A);

// * modes

bool plots(char *argv[]);

bool links(char *argv[]);

// * plots tools

#define QUEEN "Cersei Lannister"

bool compareByName(const Person *a, const Person *b);

void print_names(std::vector<Person *> friends);

void print_relations_matrix(std::vector<std::vector<int>> matrix);

bool fill_plots(std::vector<Person *> friends, std::vector<std::string> file_conspiracies);

void prints_plots(std::vector<Person *> friends);

std::vector<std::vector<int>> fn_adjacent_matrix(std::vector<Person *> people, bool plot_matrix);
