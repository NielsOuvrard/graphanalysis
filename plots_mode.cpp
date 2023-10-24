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

#define VALUE_EXISTS(vector, value) (std::find((vector).begin(), (vector).end(), (value)) != (vector).end())

bool contact_close_queen_friends(
        std::vector<std::vector<int>> matrix_plots,
        std::vector<std::vector<int>> friendships,
        int id_queen,
        int target,
        std::vector<int> &attack,
        std::vector<int> visited,
        int deep)
{
    if (deep > 3) {
        return false;
    }
    for (int i = 0; i < friendships.size(); i++) {
        if (friendships[i][id_queen] == 1 && friendships[target][i] == 1) {
            // i = close friend
            // not to convince
            std::cout << "not to convince : " << i << " to kill " << target << std::endl;
            attack.push_back(i);
            attack.push_back(target);
            return true;
        }
    }
    // if not, plotting against "enemies" of "close friends" of "target"
    for (int i = 0; i < friendships.size(); i++) {
        if (friendships[target][i] >= 0) {
            // i = "close friends" of "target"
            for (int j = 0; j < friendships.size(); j++) {
                if (target != j && matrix_plots[i][j] == 1 && !VALUE_EXISTS(visited, j)) {
                    // j = "enemies" of "close friends" of "target"
                    // std::cout << j << " is enemy of " << target << " and deep = " << deep << std::endl;
                    bool found = contact_close_queen_friends(matrix_plots, friendships, i, j, attack, visited, deep + 1);
                    attack.push_back(j);
                    if (found) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void save_queen_matrix(
        std::vector<std::vector<int>> matrix_plots,
        std::vector<std::vector<int>> friendships,
        int id_queen,
        std::vector<std::vector<int>> &list_of_attacks,
        int deep)
{
    for (int i = 0; i < matrix_plots.size(); i++) {
        if (matrix_plots[i][id_queen] >= 1 && friendships[i][id_queen] >= 1) {
            // * if enemy is friend with queen
            std::vector<int> attack;
            attack.push_back(id_queen);
            attack.push_back(i);
            list_of_attacks.push_back(attack);
        }
        else if (matrix_plots[i][id_queen] == 1) {
            // * i = enemy of the queen
            std::vector<int> attack;
            std::vector<int> visited;
            visited.push_back(id_queen);
            // std::cout << "enemy of the queen : " << i << std::endl;
            contact_close_queen_friends(matrix_plots, friendships, id_queen, i, attack, visited, deep);
            list_of_attacks.push_back(attack);
        }
    }
}

void trunc_matrix(std::vector<std::vector<int>> &matrix, int n)
{
    for (auto &line: matrix) {
        for (auto &value: line) {
            value = value > n ? 0 : value;
        }
    }
}

std::vector<std::vector<int>> fn_adjacent_matrix(std::vector<Person *> people, bool plot_matrix)
{
    std::vector<std::vector<int>> matrix;
    for (auto &person: people) {
        std::vector<int> line;
        for (auto &other_person: people) {
            bool is_friend_of_him = false;
            std::vector<Person *> friend_or_plots = plot_matrix ? person->plots : person->friends;
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

void makeSymmetric(std::vector<std::vector<int>> &matrix)
{
    int n = matrix.size();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // Reflect the value across the main diagonal
            matrix[j][i] = matrix[i][j] ? matrix[i][j] : matrix[j][i];
            matrix[i][j] = matrix[j][i];
        }
    }
}

bool plots(char *argv[])
{
    std::vector<std::string> file_friendship = file_to_vector(argv[2]);
    std::vector<std::string> file_conspiracies = file_to_vector(argv[3]);
    int n = std::stoi(argv[4]);

    std::vector<Person *> people = create_graph(file_friendship);

    if (!fill_plots(people, file_conspiracies)) {
        std::cout << "error plot to unknown person\n";
        return false;
    }

    std::sort(people.begin(), people.end(), compareByName);
    print_names(people);
    std::cout << std::endl;

    std::vector<std::vector<int>> friendships = fn_adjacent_matrix(people, false);
    std::vector<std::vector<int>> matrix_plot = fn_adjacent_matrix(people, true);
    makeSymmetric(matrix_plot);
    floydWarshall(friendships);
    trunc_matrix(friendships, n);
    // no need n after this

    print_relationships_matrix(friendships);
    // print_relationships_matrix(matrix_plot);

    std::cout << "Conspiracies:\n";
    std::vector<std::vector<int>> list_of_attacks;
    save_queen_matrix(matrix_plot, friendships, 0, list_of_attacks, 0);

    // TODO :
    // You must display the chain of treason that results on saving the Queen, sorted from the shortest to the
    // longest chains. For each chain of same length, you must display them in alphabetical order.
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
    //    std::cout << "No conspiracy possible against ";
    // std::cout << "There is only one way out: treason !\n";


    // free memory
    for (auto person: people) {
        delete person;
    }
    return true;
}
