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

int find_person(Person *p, std::vector<std::string> visited, std::string s, int deep)
{
    for (auto friend_ : p->friends)
    {
        if (friend_->name == s)
            return deep;
        bool visited_ = false;
        for (auto v : visited)
        {
            if (friend_->name == v)
                visited_ = true;
        }
        if (!visited_)
        {
            visited.push_back(friend_->name);
            return find_person(friend_, visited, s, deep + 1);
        }
    }
    return deep;
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

void links(int argc, char *argv[])
{
    std::vector<std::string> file = file_to_vector(argv[2]);
    std::string person1 = argv[3];
    std::string person2 = argv[4];

    // handling error

    std::vector<Person *> friends = create_graph(file);

    std::cout << "Degree of separation between " << person1 << " and " << person2 << ": ";

    int deep = -1;
    for (auto person : friends)
    {
        if (person->name == person1)
        {
            std::vector<std::string> visited;
            visited.push_back(person->name);
            deep = find_person(person, visited, person2, 0);
        }
    }
    std::cout << deep << std::endl;

    // free memory
    for (auto person : friends)
    {
        delete person;
    }
}
