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

bool print_separation(std::string person1, std::string person2, int deep)
{
    std::cout << "Degree of separation between " << person1 << " and " << person2 << ": " << deep << std::endl;
    return true;
}


Person *exists_person(std::vector<Person *> v, std::string s)
{
    for (auto person: v) {
        if (person->name == s)
            return person;
    }
    return nullptr;
}

std::vector<Person *> create_graph_and_find(std::string filename, Person **start, Person **target, std::string person1, std::string person2)
{
    if (filename.empty()) {
        std::cerr << "Error: argument is not a valid string." << std::endl;
        exit(84);
    }
    std::ifstream inputFile(filename);

    // Check if the file was successfully opened
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        exit(84);
    }

    // Read lines from the file and store them in the vector
    std::vector<Person *> friends;
    std::string line;
    while (std::getline(inputFile, line)) {// Find the position of " is friends with " in the line
        size_t pos = line.find(" is friends with ");

        // Check if the substring is not found in the line
        if (pos == std::string::npos) {
            // If not found, the file is inconsistent
            std::cerr << "File is inconsistent." << std::endl;
            exit(84);
        }

        // Extract the names of the friends from the line
        std::string friend1 = line.substr(0, pos);
        std::string friend2 = line.substr(pos + 17);// Assuming you want to skip the substring itself

        // Check if friend1 and friend2 exist in the 'friends' container
        Person *p1 = exists_person(friends, friend1);
        if (!p1) {
            // If friend1 doesn't exist, create a new Person object for friend1
            Person *p = new Person();
            p->name = friend1;
            if (!(*start) && friend1 == person1) {
                (*start) = p;
            }
            else if (!(*target) && friend1 == person2) {
                (*target) = p;
            }
            p1 = p;
            friends.push_back(p);
        }

        Person *p2 = exists_person(friends, friend2);
        if (!p2) {
            // If friend2 doesn't exist, create a new Person object for friend2
            Person *p = new Person();
            p->name = friend2;
            if (!(*start) && friend2 == person1) {
                (*start) = p;
            }
            else if (!(*target) && friend2 == person2) {
                (*target) = p;
            }
            p2 = p;
            friends.push_back(p);
        }

        // Add friend2 to the friends list of friend1, and vice versa
        p1->friends.push_back(p2);
        p2->friends.push_back(p1);
    }
    return friends;
}

bool links(char *argv[])
{
    std::string person1 = argv[3];
    std::string person2 = argv[4];
    if (person1 == person2)
        return print_separation(person1, person2, 0);
    Person *start = nullptr;
    Person *target = nullptr;
    std::vector<Person *> people = create_graph_and_find(argv[2], &start, &target, person1, person2);

    if (!start || !target)
        return print_separation(person1, person2, -1);

    std::sort(people.begin(), people.end(), compareByName);
    int id_start = 0;
    for (auto person: people) {
        if (person->name == person1) {
            start = person;
            break;
        }
        id_start++;
    }
    int id_target = 0;
    for (auto person: people) {
        if (person->name == person2) {
            target = person;
            break;
        }
        id_target++;
    }
    if (!start || !target)// impossible, checked before
        return false;


    std::vector<std::vector<int>> friendships = fn_adjacent_matrix(people, false);
    floydWarshall(friendships);

    print_separation(person1, person2, friendships[id_start][id_target]);

    // free memory
    for (auto person: people) {
        delete person;
    }
    return true;
}

/*
inks friends_example "Cersei Lannister" "Vere"
Degree of separation between Cersei Lannister and Vere: 3
./game_of_graphs --links friends_example "Cersei Lannister" "Vere"  0.00s user 0.00s system 67% cpu 0.006 total
➜  B-CNA-500-MAR-5-1-graphanalysis-leo2.martin git:(main) ✗ time ./game_of_graphs --links friends_example "Cersei Lannister" "Vere"
Degree of separation between Cersei Lannister and Vere: 3
./game_of_graphs --links friends_example "Cersei Lannister" "Vere"  0.00s user 0.00s system 67% cpu 0.007 total
➜  B-CNA-500-MAR-5-1-graphanalysis-leo2.martin git:(main) ✗ time ./game_of_graphs --links friends_example "Cersei Lannister" "Vere"
Degree of separation between Cersei Lannister and Vere: 3
./game_of_graphs --links friends_example "Cersei Lannister" "Vere"  0.00s user 0.00s system 68% cpu 0.007 total
➜  B-CNA-500-MAR-5-1-graphanalysis-leo2.martin git:(main) ✗ time ./game_of_graphs --links friends_example "Cersei Lannister" "Vere"
Degree of separation between Cersei Lannister and Vere: 3
./game_of_graphs --links friends_example "Cersei Lannister" "Vere"  0.00s user 0.00s system 67% cpu 0.007 total
➜  B-CNA-500-MAR-5-1-graphanalysis-leo2.martin git:(main) ✗ time ./game_of_graphs --links friends_example "Cersei Lannister" "Vere"
Degree of separation between Cersei Lannister and Vere: 3
./game_of_graphs --links friends_example "Cersei Lannister" "Vere"  0.00s user 0.00s system 68% cpu 0.007 total
➜  B-CNA-500-MAR-5-1-graphanalysis-leo2.martin git:(main) ✗ time ./game_of_graphs --links friends_example "Maester Aemon" "Missandej"
Degree of separation between Maester Aemon and Missandej: -1
./game_of_graphs --links friends_example "Maester Aemon" "Missandej"  0.00s user 0.00s system 66% cpu 0.006 total
➜  B-CNA-500-MAR-5-1-graphanalysis-leo2.martin git:(main) ✗ time ./game_of_graphs --links friends_example "Maester Aemon" "Missandei"
Degree of separation between Maester Aemon and Missandei: 5
./game_of_graphs --links friends_example "Maester Aemon" "Missandei"  0.00s user 0.00s system 67% cpu 0.006 total


links friends_example "Maester Aemon" "Missandei"
Degree of separation between Maester Aemon and Missandei: 5
./game_of_graphs --links friends_example "Maester Aemon" "Missandei"  0.00s user 0.00s system 2% cpu 0.263 total

Degree of separation between Cersei Lannister and Vere: 3
./game_of_graphs --links friends_example "Cersei Lannister" "Vere"  0.00s user 0.00s system 52% cpu 0.005 total

Degree of separation between Cersei Lannister and Vere: 3
./game_of_graphs --links friends_example "Cersei Lannister" "Vere"  0.00s user 0.00s system 4% cpu 0.163 total

➜  B-CNA-500-MAR-5-1-graphanalysis-leo2.martin git:(main) ✗ time ./game_of_graphs --links friends_example "Cersei Lannister" "Vere"
Degree of separation between Cersei Lannister and Vere: 3
./game_of_graphs --links friends_example "Cersei Lannister" "Vere"  0.00s user 0.00s system 63% cpu 0.007 total
➜  B-CNA-500-MAR-5-1-graphanalysis-leo2.martin git:(main) ✗ time ./game_of_graphs --links friends_example "Cersei Lannister" "Vere"
Degree of separation between Cersei Lannister and Vere: 3
./game_of_graphs --links friends_example "Cersei Lannister" "Vere"  0.00s user 0.00s system 64% cpu 0.005 total
➜  B-CNA-500-MAR-5-1-graphanalysis-leo2.martin git:(main) ✗ time ./game_of_graphs --links friends_example "Cersei Lannister" "Vere"
Degree of separation between Cersei Lannister and Vere: 3
./game_of_graphs --links friends_example "Cersei Lannister" "Vere"  0.00s user 0.00s system 64% cpu 0.007 total
*/