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

void contact_close_queen_friends(
        std::vector<std::vector<int>> plots,
        std::vector<std::vector<int>> friendships,
        uint16_t id_queen,
        uint16_t target,
        std::vector<Person *> people)
{
    for (int i = 0; i < friendships.size(); i++) {
        if (friendships[i][id_queen] == 1) {
            // friend of queen
//            std::cout << "contact " << i << " is a friend of QUEEN " << std::endl;

            if (friendships[target][i] == 1) {
                // target
//                std::cout << "contact " << i << " is a friend of " << target << std::endl;
                std::cout << people[i]->name << " -> " << people[target]->name << std::endl;
            }
        }
    }
}

void save_queen_matrix(
        std::vector<std::vector<int>> plots,
        std::vector<std::vector<int>> friendships,
        uint16_t id_queen,
        std::vector<Person *> people)
{
    for (int i = 0; i < plots.size(); i++) {
        if (plots[i][id_queen] == 1) {
//            std::cout << i << " is plotting against QUEEN " << std::endl;
            contact_close_queen_friends(plots, friendships, id_queen, i, people);
        }
    }
}

std::vector<std::vector<int>> fn_adjacent_matrix(std::vector<Person *> people, bool plot)
{
    std::vector<std::vector<int>> matrix;
    for (auto &person: people) {
        std::vector<int> line;
        for (auto &other_person: people) {
            bool is_friend_of_him = false;
            std::vector<Person *> friend_or_plots = plot ? person->plots : person->friends;
            for (auto &friend_of_him: friend_or_plots) {
                if (other_person->name == friend_of_him->name) {
                    line.push_back(1);
                    is_friend_of_him = true;
                }
            }
            if (!is_friend_of_him) {
                line.push_back(0);
            }
        }
        matrix.push_back(line);
    }
    return matrix;
}

bool plots(int argc, char *argv[])
{
    std::vector<std::string> file_friendship = file_to_vector(argv[2]);
    std::vector<std::string> file_conspiracies = file_to_vector(argv[3]);
    int max_length_of_friendship_paths = std::stoi(argv[4]);

    std::vector<Person *> people = create_graph(file_friendship);

    if (!fill_plots(people, file_conspiracies)) {
        std::cout << "error plot to unknown person\n";
        return false;
    }

    std::sort(people.begin(), people.end(), compareByName);
    print_names(people);
    std::cout << std::endl;

    std::vector<std::vector<int>> matrix_friendship = fn_adjacent_matrix(people, false);
    std::vector<std::vector<int>> matrix_plot = fn_adjacent_matrix(people, true);
    floydWarshall(matrix_friendship);
    print_relationships_matrix(matrix_friendship, max_length_of_friendship_paths);
    print_relationships_matrix(matrix_plot, max_length_of_friendship_paths);

    std::cout << "Conspiracies:\n";
    save_queen_matrix(matrix_plot, matrix_friendship, 0, people);

    std::cout << "\nResult:\n";
    std::cout << "The Crown is safe !\n";


    // free memory
    for (auto person: people) {
        delete person;
    }
    return true;
}
