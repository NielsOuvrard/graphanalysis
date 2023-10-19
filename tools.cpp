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
int find_person(Person *start, std::vector<std::string> &visited, const std::string &target, int deep)
{
    // Check if the current person is the target person.
    if (start->name == target)
    {
        return deep; // Return the depth when the target is found.
    }

    // Initialize the shortest link to a large value.
    int shortestLink = -1;

    // Iterate through the friends of the current person.
    for (auto friend_ : start->friends)
    {
        // Check if this friend has already been visited.
        if (std::find(visited.begin(), visited.end(), friend_->name) == visited.end())
        {
            // Mark the friend as visited.
            visited.push_back(friend_->name);

            // Recursively search for the target person in the friend's network.
            int result = find_person(friend_, visited, target, deep + 1);

            // If the result is not -1, the target person was found in this branch.
            if (result != -1)
            {
                if (shortestLink == -1 || result < shortestLink)
                {
                    shortestLink = result; // Update the shortest link found.
                }
            }
        }
    }

    return shortestLink; // Return the shortest link found, or -1 if no path is found.
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
