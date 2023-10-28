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
    if (deep > 10) {
        return false;
    }
    // queen = Tom
    // target = Elon

    // joris -> Elon
    // Tom -> Thomas

    //    Joris->Elon->Tom->Thomas
    // std::cout
    //         << " - - QUEEN \033[1;32m"
    //         << people[id_queen]->name
    //         << "\033[0m - target \033[1;32m"
    //         << people[target]->name
    //         << "\033[0m"
    //         << std::endl;


    //    for (auto &vis: visited) {
    //        std::cout << " # " << people[vis]->name;
    //    }
    //    std::cout << std::endl;

    // The Mountain -> Jon Snow -> Theon Greyjoy -> Sansa Stark

    // ! foreach friend of queen, plotting against the target
    for (int i = 0; i < friendships.size(); i++) {
        // a close friend can kill target ?
        // std::cout << " \t" << people[i]->name;
        // std::cout << " f[i][target] = " << friendships[target][i] << std::endl;
        if (i != id_queen && i != target && !VALUE_EXISTS(visited, i)
            // ! here, fix D -> A : we didn't create plot, just active an kill plot && friendships[id_queen][i] >= 0
            // ! && friendships[target][i] >= 1 // no because  Tywin Lannister kill Daenerys Targaryen
            //            && friendships[id_queen][i] >= 1 // break everything, didn't work with QUEEN C - target D
            && (friendships[id_official_queen][i] >= 1 || i == id_official_queen)// only friend of the official queen, or be the official queen
            //&& matrix_plots[i][id_queen] == 0                                    // not sure
            && matrix_plots[i][target] == 1// working with C kill B
        ) {                                // friendships[id_queen][i] >= 1 &&
                                           // ! tell him to kill
                                           // i = close friend
                                           // ? not to convince
            // std::cout << " - - - \033[1;32m"
            //           << people[i]->name
            //           << "\033[0m is a common friend of target \033[1;32m"
            //           << people[target]->name
            //           << "\033[0m and a common friend of queen \033[1;32m"
            //           << people[id_queen]->name
            //           << "\033[0m"
            //           << std::endl;
            attack.push_back(i);
            attack.push_back(target);

            //            std::cout << " # " << deep << std::endl;
            //            for (auto &ata: attack) {
            //                std::cout << " # " << people[ata]->name;
            //            }
            return true;
        }
    }
    // if not, plotting against "enemies" of "close friends" of "target"
    // ! foreach friend of target
    for (int i = 0; i < friendships.size(); i++) {
        //        if (!VALUE_EXISTS(visited, i) && i != target && friendships[target][i] >= 0) { // !  >= 0 ?!

        if (!VALUE_EXISTS(visited, i) && i != target && friendships[target][i] > 0) {
            // i = "close friends" of "target"
            // std::cout
            //         << "\033[1;32m"
            //         << people[i]->name
            //         << "\033[0m is a close friend of \033[1;32m"
            //         << people[target]->name
            //         << "\033[0m = "
            //         << friendships[target][i]
            //         << std::endl;
            // ! foreach "enemies" of "close friends" of "target"
            // std::cout << "who is plotting agains " << people[i]->name << " ? \n";
            for (int j = 0; j < friendships.size(); j++) {
                // std::cout << matrix_plots[j][i] << " is plotting agains " << people[j]->name << " ! " << std::endl;
                if (!VALUE_EXISTS(visited, j) && target != j && i != j && matrix_plots[j][i] == 1 && j != id_official_queen && i != id_official_queen) {
                    // j = "enemies" of "close friends" of "target"
                    // std::cout
                    //         << " - \033[1;32m"
                    //         << people[j]->name// Elon
                    //         << "\033[1;31m want to kill \033[1;32m"
                    //         << people[i]->name// Tom
                    //         << "\033[0m and deep = \033[1;31m"
                    //         << deep
                    //         << "\033[0m"
                    //         << std::endl;
                    visited.push_back(target);

                    bool found = contact_close_queen_friends(matrix_plots, friendships, i, j, visited, attack, deep + 1);
                    if (found) {
                        attack.push_back(i);
                        attack.push_back(target);
                        return true;
                    }
                    else {
                        //                        std::cout << " - \033[1;31m echec\n";
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
    // ! for each enemy of the queen
    for (int i = 0; i < matrix_plots.size(); i++) {
        //        if (matrix_plots[i][id_queen] == 1 && friendships[i][id_queen] == 1) {
        //            // ! if enemy is friend with queen
        //            std::vector<int> attack;
        //            attack.push_back(id_queen);
        //            attack.push_back(i);
        //            list_of_attacks.push_back(attack);
        //        } else
        if (matrix_plots[i][id_queen] == 1) {
            // ! i = enemy of the queen
            std::vector<int> attack;
            std::vector<int> visited;
            for (auto &enemy: enemy_visited) {
                visited.push_back(enemy);
            }
            //            visited.push_back(id_queen);
            //            std::cout << "enemy of the queen : " << people[i]->name << " - " << people[id_queen]->name << std::endl;
            //            std::cout << "visited : " << (enemy_visited.size() > 0 ? enemy_visited.back() : -1) << std::endl;
            bool success_local = contact_close_queen_friends(matrix_plots, friendships, id_queen, i, visited, attack, deep);
            if (!success_local) {
                //                std::sort(list_of_attacks.begin(), list_of_attacks.end(), compareBySize);
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

            //            for (auto &ata: attack) {
            //                std::cout << " # " << people[ata]->name << std::endl;
            //            }

            list_of_attacks.push_back(attack);

            // ! this enemy is dead
            // enemy_visited.push_back(i);// ? maybe not this, maybe someone who plot to queen can not change mind
            // ? maybe begin by check nearest killer
        }
    }
    return true;
}
//Jon Snow -> Tywin Lannister
//Theon Greyjoy -> Sansa Stark
//Theon Greyjoy -> Sansa Stark -> Cersei Lannister -> Daenerys Targaryen


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
    // std::vector<Person *>
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
    // no need n after this

    print_relationships_matrix(friendships);
    // print_relationships_matrix(matrix_plot);

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
    // TODO :
    // You must display the chain of treason that results on saving the Queen, sorted from the shortest to the
    // longest chains. For each chain of same length, you must display them in alphabetical order.
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
    //    std::cout << "No conspiracy possible against ";
    // std::cout << "There is only one way out: treason !\n";


    // free memory
    for (auto person: people) {
        delete person;
    }
    return true;
}
