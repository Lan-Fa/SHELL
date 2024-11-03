#include "main.h"

#include <iostream>
#include <filesystem>


void exec_rm(const std::string& filename)
{
    try
    {
        if (!std::filesystem::exists(filename))
        {
            exit_code = 1;
            std::cerr << "File " << filename << " does not exist" << std::endl;
            return;
        }
        std::filesystem::remove(filename);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

int main(const int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        exit(1);
    }

    for(int i = 1; i < argc; i++) exec_rm(argv[i]);

    return exit_code;
}
