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
