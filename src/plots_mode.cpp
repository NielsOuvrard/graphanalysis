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

std::vector<Person *> people;
int id_official_queen = -1;

#define VALUE_EXISTS(vector, value) (std::find((vector).begin(), (vector).end(), (value)) != (vector).end())


bool compareBySize(std::vector<int> a, std::vector<int> b)
{
    if (a.size() > b.size())
        return false;
    else if (a.size() < b.size())
        return true;
    if (people[a[0]]->name < people[b[0]]->name)
        return true;
    else
        return false;
}


bool contact_close_queen_friends(
        std::vector<std::vector<int>> matrix_plots,
        std::vector<std::vector<int>> friendships,
        int id_queen,
        int target,
        std::vector<int> visited,
        std::vector<int> &attack,
        int deep)
{
    for (int i = 0; i < friendships.size(); i++) {
        if (i != id_queen && i != target && !VALUE_EXISTS(visited, i) && (friendships[id_official_queen][i] >= 1 || i == id_official_queen) && matrix_plots[i][target] == 1) {
            attack.push_back(i);
            attack.push_back(target);
            return true;
        }
    }

    for (int i = 0; i < friendships.size(); i++) {
        if (!VALUE_EXISTS(visited, i) && i != target && friendships[target][i] > 0) {
            for (int j = 0; j < friendships.size(); j++) {
                if (!VALUE_EXISTS(visited, j) && target != j && i != j && matrix_plots[j][i] == 1 && j != id_official_queen && i != id_official_queen) {
                    visited.push_back(target);
                    bool found = contact_close_queen_friends(matrix_plots, friendships, i, j, visited, attack, deep + 1);
                    if (found) {
                        attack.push_back(i);
                        attack.push_back(target);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool save_queen_matrix(
        std::vector<std::vector<int>> matrix_plots,
        std::vector<std::vector<int>> friendships,
        int id_queen,
        std::vector<std::vector<int>> &list_of_attacks,
        int deep)
{
    std::vector<int> enemy_visited;
    bool success = true;

    for (int i = 0; i < matrix_plots.size(); i++) {
        if (matrix_plots[i][id_queen] == 1) {

            std::vector<int> attack;
            std::vector<int> visited;
            for (auto &enemy: enemy_visited) {
                visited.push_back(enemy);
            }
            bool success_local = contact_close_queen_friends(matrix_plots, friendships, id_queen, i, visited, attack, deep);
            if (!success_local) {

                for (auto &attack: list_of_attacks) {
                    for (int i = 0; i < attack.size(); i++) {
                        std::cout << people[attack[i]]->name;
                        if (i + 1 < attack.size()) {
                            std::cout << " -> ";
                        }
                    }
                    std::cout << std::endl;
                }
                std::cout << "No conspiracy possible against " << people[i]->name;
                std::cout << "\n\nResult:\nThere is only one way out: treason !\n";
                return false;
            }
            list_of_attacks.push_back(attack);
        }
    }
    return true;
}


void trunc_matrix(std::vector<std::vector<int>> &matrix, int n)
{
    for (auto &line: matrix) {
        for (auto &value: line) {
            value = value > n ? 0 : value;
        }
    }
}

std::vector<std::vector<int>> fn_adjacent_matrix(std::vector<Person *> all_people, bool plot_matrix)
{
    std::vector<std::vector<int>> matrix;
    for (auto &person: all_people) {
        std::vector<int> line;
        for (auto &other_person: all_people) {
            bool is_friend_of_him = false;
            std::vector<Person *> friend_or_plots = plot_matrix ? person->plots : person->friends;
            for (auto &friend_of_him: friend_or_plots) {
                if (other_person->name == friend_of_him->name) {
                    line.push_back(1);
                    is_friend_of_him = true;
                    break;
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

bool plots(char *argv[])
{

    people = create_graph(argv[2]);
    std::vector<std::string> file_conspiracies = file_to_vector(argv[3]);
    int n;
    try {
        n = std::stoi(argv[4]);
    } catch (std::exception &e) {
        std::cout << "error: " << e.what() << std::endl;
        return false;
    }
    if (n < 0) {
        std::cout << "error: n must be positive" << std::endl;
        return false;
    }


    if (!fill_plots(people, file_conspiracies)) {
        std::cout << "error plot to unknown person\n";
        return false;
    }

    std::sort(people.begin(), people.end(), compareByName);
    print_names(people);
    std::cout << std::endl;

    std::vector<std::vector<int>> friendships = fn_adjacent_matrix(people, false);
    std::vector<std::vector<int>> matrix_plot = fn_adjacent_matrix(people, true);
    floydWarshall(friendships);
    trunc_matrix(friendships, n);

    std::cout << "Relationships:\n";
    print_relations_matrix(friendships);

    std::cout << "Plots:\n";
    print_relations_matrix(matrix_plot);


    int id_queen = 0;
    for (auto &person: people) {
        if (person->name == QUEEN) {
            break;
        }
        id_queen++;
    }
    id_official_queen = id_queen;

    std::cout << "Conspiracies:\n";
    std::vector<std::vector<int>> list_of_attacks;
    bool success = save_queen_matrix(matrix_plot, friendships, id_queen, list_of_attacks, 0);
    if (!success)
        return true;


    std::sort(list_of_attacks.begin(), list_of_attacks.end(), compareBySize);
    for (auto &attack: list_of_attacks) {
        for (int i = 0; i < attack.size(); i++) {
            std::cout << people[attack[i]]->name;
            if (i + 1 < attack.size()) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "\nResult:\n";
    std::cout << "The Crown is safe !\n";


    for (auto person: people) {
        delete person;
    }
    return true;
}
