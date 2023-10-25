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

//  friendships report (FR)
//  conspiracy report (CR)

// “Links” mode: it will take one file (FR) and two people as input, and displays the degree of separation
// of these people (or -1 if they are not connected).

// ./game_of_graphs --links friends_example "Vere" "Cersei Lannister"

std::vector<std::string> file_to_vector(std::string filename)
{
    std::ifstream inputFile(filename);

    // Check if the file was successfully opened
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        exit(84);
    }

    // Check if the file is empty
    if (inputFile.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "File is empty." << std::endl;
        exit(84);
    }

    // Create a vector to store the lines from the file
    std::vector<std::string> lines;

    // Read lines from the file and store them in the vector
    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    // Check if the file is inconsistent by patern matching the lines
    check_inconsistent_content(lines);

    // Close the file
    inputFile.close();
    return lines;
}

void check_inconsistent_content(std::vector<std::string> lines)
{
    for (std::string line : lines) {
        if (line.find("is friends with") == std::string::npos && line.find("is plotting against") == std::string::npos) {
            std::cerr << "File is inconsistent." << std::endl;
            exit(84);
        }
    }
    return;
}

// error handling
// file plot in frendship report
// no file
// person does not exist, queen does not exist
// is friend with himself
// no "is friend with" or "is plotting with"
// if everyone in plot file exists in friendship file

// inconsistent content
// links - empty file
// plots - empty file

void help(void)
{
    std::cout << "USAGE" << std::endl;
    std::cout << "\t./game_of_graphs [--links fr p1 p2 | --plots fr cr n]" << std::endl
              << std::endl;

    std::cout << "DESCRIPTION" << std::endl;
    std::cout << "\tfr\tfile containing friendship relations between people" << std::endl;
    std::cout << "\tpi\tname of someone in the friendships file" << std::endl;
    std::cout << "\tcr\tfile containing conspiracies intentions" << std::endl;
    std::cout << "\tn\tmaximum length of friendship paths" << std::endl;
}

int main(int argc, char **argv)
{
    if (argc == 2 && std::string(argv[1]) == "--help") {
        help();
        return 0;
    }
    else if (argc != 5) {
        std::cerr << "Invalid number of arguments" << std::endl;
        return 84;
    }

    std::string arg2(argv[2]);
    if (arg2.empty()) {
        std::cerr << "Error: argument 2 is not a valid string." << std::endl;
        exit(84);
    }

    std::string arg3(argv[3]);
    if (arg3.empty()) {
        std::cerr << "Error: argument 3 is not a valid string." << std::endl;
        exit(84);
    }

    try {
        int arg4 = std::stoi(argv[4]);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: argument 4 is not a valid integer." << std::endl;
        exit(84);
    }
    if (std::string(argv[1]) == "--links") {
        return links(argv) ? 0 : 84;
    }
    else if (std::string(argv[1]) == "--plots") {
        return plots(argv) ? 0 : 84;
    }

    return 84;
}
