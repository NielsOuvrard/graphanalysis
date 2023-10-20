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

std::vector<std::vector<int>> fn_adjacent_matrix(std::vector<Person *> people)
{
    std::vector<std::vector<int>> matrix;
    for (auto &person : people)
    {
        std::vector<int> line;
        for (auto &other_person : people)
        {
            bool is_friend_of_him = false;
            for (auto &friend_of_him : person->friends)
            {
                if (other_person->name == friend_of_him->name)
                {
                    line.push_back(1);
                    is_friend_of_him = true;
                }
            }
            if (!is_friend_of_him)
            {
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

    if (!fill_plots(people, file_conspiracies))
    {
        std::cout << "error plot to unknown person\n";
        return false;
    }

    std::sort(people.begin(), people.end(), compareByName);
    print_names(people);

    std::cout << std::endl;

    std::vector<std::vector<int>> matrix_adj = fn_adjacent_matrix(people);
    //    print_relationships_matrix(matrix_adj, 2000);
    floydWarshall(matrix_adj);
    print_relationships_matrix(matrix_adj, max_length_of_friendship_paths);

    //    std::vector<std::vector<int>> matrix = create_matrix_from_relationships(people);
    //    print_relationships_matrix(matrix, max_length_of_friendship_paths);

    Person *queen = nullptr;
    for (auto &person : people)
    {
        if (person->name == QUEEN)
            queen = person;
    }

    // stop_killing_queen(people, queen);

    // free memory
    for (auto person : people)
    {
        delete person;
    }
    return true;
}
