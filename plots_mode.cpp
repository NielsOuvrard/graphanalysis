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

#define QUEEN "Cersei Lannister"

// You need to build a graph representing the conspiracies between each political figures (who plots against
// whom). Using this graph, you will need to stop people plotting against the Queen (called direct enemies),
// by plotting against them.

// To do so, you need to contact every “close” friend to the Crown that is plotting against these enemies.
// If an enemy has only non-close friends plotting against them, you can still convince these non-close friends by
// plotting against their enemies.
// * You will need to check who’s plotting against them. These people are “indirect enemies”, and just like your
// direct enemies, you can plot with their enemies if the latters are “close” friends. If you can plot against at
// least one of their enemies, then this “non-close friend” will help you, just like a “close friend”.
// You must continue until finding a way to plot (directly or indirectly) against all your direct enemies

// * A person is considered a “close friend” if their friendship level relative to the Queen is less than or equal to n.

// You must display the chain of treason that results on saving the Queen, sorted from the shortest to the
// longest chains. For each chain of same length, you must display them in alphabetical order.

// If you can conspire with multiple allies, choose in priority the ones that are closest to the Queen, then those
// who are the less threatening towards you (i.e.: less flagged as direct or indirect enemies) and finally take the
// first one in alphabetical order.
// If you can successfully plot against all the direct enemies, you must print “The Crown is safe !”, otherwise
// you need to betray the Crown and must display “There is only one way out: treason !”.

bool compareByName(const Person *a, const Person *b)
{
    return a->name < b->name;
}

void print_names(std::vector<Person *> friends)
{
    std::cout << "Names:\n";
    for (auto person : friends)
    {
        std::cout << person->name << std::endl;
    }
}

std::vector<std::vector<int>> create_matrix_from_relationships(std::vector<Person *> friends)
{
    std::vector<std::vector<int>> matrix;

    for (auto start : friends)
    {
        std::vector<int> new_line;
        for (auto target : friends)
        {
            new_line.push_back(find_length_of_shortest_path_between_two_nodes_Person(start, target));
        }
        matrix.push_back(new_line);
    }
    return matrix;
}

void print_relationships_matrix(std::vector<std::vector<int>> matrix, int max_length_of_friendship_paths)
{
    std::cout << "Relationships:\n";
    for (auto y : matrix)
    {
        for (int i = 0; i < y.size(); i++)
        {
            if (y[i] <= max_length_of_friendship_paths)
                std::cout << y[i];
            else
                std::cout << "0";
            if (i + 1 != y.size())
                std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool fill_plots(std::vector<Person *> friends, std::vector<std::string> file_conspiracies)
{
    for (auto &line : file_conspiracies)
    {
        std::string enemy1 = line.substr(0, line.find(" is plotting against "));
        std::string enemy2 = line.substr(line.find(" is plotting against ") + 21, line.length());

        Person *p1 = nullptr;
        Person *p2 = nullptr;
        for (auto &person : friends)
        {
            if (person->name == enemy1)
                p1 = person;
            else if (person->name == enemy2)
                p2 = person;
        }
        if (!p1 || !p2)
        {
            std::cout << enemy1 << " or " << enemy2 << " didn't found\n";
            return false;
        }
        p1->plots.push_back(p2);
    }
    return true;
}

void prints_plots(std::vector<Person *> friends)
{
    for (auto person : friends)
    {
        if (person->plots.size() != 0)
        {
            std::cout << person->name << " is plotting against :\n";
            for (auto enemy : person->plots)
            {
                std::cout << "\t" << enemy->name << std::endl;
            }
            std::cout << std::endl;
        }
    }
}

void stop_killing_queen(std::vector<Person *> people, Person *queen)
{
    for (auto &queen_enemy : people)
    {
        for (auto &target : queen_enemy->plots)
        {
            if (target->name == QUEEN)
            {
                // std::cout << queen_enemy->name << " want to kill the queen\n";
                // either, kill him
                // either, find someone to kill him
                for (auto &queen_friend : people)
                {
                    for (auto &target_of_queen_friend : queen_friend->plots)
                    {
                        if (target_of_queen_friend->name == queen_enemy->name)
                        {
                            std::cout << target_of_queen_friend->name << " -> " << queen_enemy->name << std::endl;
                        }
                    }
                }
            }
        }
    }
}

bool plots(int argc, char *argv[])
{
    std::vector<std::string> file_friendship = file_to_vector(argv[2]);
    std::vector<std::string> file_conspiracies = file_to_vector(argv[3]);
    int max_length_of_friendship_paths = std::stoi(argv[4]);

    std::vector<Person *> friends = create_graph(file_friendship);

    if (!fill_plots(friends, file_conspiracies))
    {
        std::cout << "error plot to unknown person\n";
        return false;
    }

    std::sort(friends.begin(), friends.end(), compareByName);
    print_names(friends);

    std::cout << std::endl;

    std::vector<std::vector<int>> matrix = create_matrix_from_relationships(friends);
    print_relationships_matrix(matrix, max_length_of_friendship_paths);

    Person *queen = nullptr;
    for (auto &person : friends)
    {
        if (person->name == QUEEN)
            queen = person;
    }

    // stop_killing_queen(friends, queen);

    // free memory
    for (auto person : friends)
    {
        delete person;
    }
    return true;
}
