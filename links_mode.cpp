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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

std::vector<std::string> file_to_vector(std::string filename);

class Person
{
public:
    std::string name;
    std::vector<Person *> friends;
    // std::vector<Person> plots;
};

bool exists(std::vector<Person *> v, std::string s)
{
    for (auto person : v)
    {
        if (person->name == s)
            return true;
    }
    return false;
}


/**
 * @brief Find the shortest link between two persons in a social network.
 *
 * This function explores the social network graph to find the shortest link between two persons.
 *
 * @param start - The starting person to search from.
 * @param visited - A vector to keep track of visited persons to avoid revisiting.
 * @param target - The name of the person to find.
 * @param deep - The current depth or distance from the starting person.
 *
 * @return The shortest link (depth) between the starting person and the target person,
 * or -1 if the target person is not found.
 */
int find_person(Person* start, std::vector<std::string> visited, const std::string& target, int deep = 0) {
    // Check if the current person is the target person.
    if (start->name == target) {
        return deep; // Return the depth when the target is found.
    }

    // Iterate through the friends of the current person.
    for (auto friend_ : start->friends) {
        // Check if this friend has already been visited.
        if (std::find(visited.begin(), visited.end(), friend_->name) == visited.end()) {
            // Mark the friend as visited.
            visited.push_back(friend_->name);

            // Recursively search for the target person in the friend'target network.
            int result = find_person(friend_, visited, target, deep + 1);

            // If the result is not -1, the target person was found in this branch.
            if (result != -1) {
                return result; // Return the shortest link found.
            }
        }
    }

    // If the target person is not found in this branch, return -1.
    return -1;
}


std::vector<Person *> create_graph(std::vector<std::string> file)
{
    std::vector<Person *> friends;

    for (auto line : file)
    {
        std::string friend1 = line.substr(0, line.find(" is friends with "));
        std::string friend2 = line.substr(line.find(" is friends with ") + 17, line.length());

        if (!exists(friends, friend1))
        {
            Person *p = new Person();
            p->name = friend1;
            friends.push_back(p);
        }
        if (!exists(friends, friend2))
        {
            Person *p = new Person();
            p->name = friend2;
            friends.push_back(p);
        }
        Person *p1;
        Person *p2;

        for (auto &person : friends)
        {
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
    for (auto person : friends)
    {
        std::cout << person->name << std::endl;
        std::cout << "friends: ";
        for (auto friend_ : person->friends)
        {
            std::cout << friend_->name << ", ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
}

int print_separation(std::string person1, std::string person2, int deep)
{
    std::cout << "Degree of separation between " << person1 << " and " << person2 << ": " << deep << std::endl;
    return deep;
}

int links(int argc, char *argv[])
{
    std::vector<std::string> file = file_to_vector(argv[2]);
    std::string person1 = argv[3];
    std::string person2 = argv[4];

    // handling error
    if (person1 == person2)
        return print_separation(person1, person2, 0);

    std::vector<Person *> friends = create_graph(file);

    if (!exists(friends, person1) || !exists(friends, person2))
        return print_separation(person1, person2, -1);

    std::cout << "Degree of separation between " << person1 << " and " << person2 << ": ";

    Person *start = nullptr;
    for (auto person : friends)
    {
        if (person->name == person1)
            start = person;
    }
    if (!start)// impossible, checked before
        return 0;
    std::vector<std::string> visited;
    int deep = find_person(start, visited, person2);
    print_separation(person1, person2, deep - 1);

    // free memory
    for (auto person : friends)
    {
        delete person;
    }
    return 0;
}
