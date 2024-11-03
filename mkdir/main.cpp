#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>

#include "main.h"

bool create_dir(const std::string& path)
{
    if(mkdir(path.c_str(), 0755) == 0)
    {
        return true;
    } else
    {
        std::cerr << "Failed to create directory: " << path << std::endl;
        return false;
    }
}


inline void exec_mkdir(const std::string& path)
{
    int cur = 0;

    while((cur = path.find('/', cur)) != std::string::npos)
    {
        if(!create_dir(path.substr(0, cur++)))
        {
            exit_code = 1;
            return;
        }
    }

    exit_code = create_dir(path) ? 0 : 1;
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        exit_code = 1;
        goto exit;
    }

    exec_mkdir(argv[1]);

    exit:
    return exit_code;
}
