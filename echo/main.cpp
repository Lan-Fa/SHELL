#include <iostream>

#include "main.h"

int main(const int argc, char **argv)
{
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <text>" << std::endl;
        return 1;
    }

    for(int i = 1; i < argc; i++)
    {
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl;
    return exit_code;
}
