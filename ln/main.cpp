#include "main.h"

#include <cstring>
#include <iostream>
#include <filesystem>

void create_hard_link(const char* TARGET, const char* LINK_NAME)
{
    try {
        std::filesystem::create_hard_link(TARGET, LINK_NAME);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
        exit_code = 1;
    }
}

void create_symbolic_link(const char* TARGET, const char* LINK_NAME)
{
    try {
        std::filesystem::create_symlink(TARGET, LINK_NAME);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
        exit_code = 1;
    }
}

int main(const int argc, char **argv)
{
    if(argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " [-s] <target> <link>" << std::endl;
    }

    bool is_symbolic_link = false;
    if(strcmp(argv[1], "-s") == 0) is_symbolic_link = true;
    const char* TARGET = is_symbolic_link ? argv[2] : argv[1];
    const char* LINK_NAME = is_symbolic_link ? argv[3] : argv[2];

    if(is_symbolic_link) create_symbolic_link(TARGET, LINK_NAME);
    else create_hard_link(TARGET, LINK_NAME);

    return exit_code;
}

