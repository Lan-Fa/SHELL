#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "main.h"

#include <cstring>
#include <sys/time.h>

void exec_touch(const std::string& filename)
{
    int res = utimes(filename.c_str(), nullptr);

    if(res == -1)
    {
        std::ofstream new_file(filename);
        if(!new_file)
        {
            std::cerr << "Failed to create file: " << filename << std::endl;
            exit_code = 1;
            return;
        }
        new_file.close();
    }
}


int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        exit_code = 1;
        goto exit;
    }
    exec_touch(argv[1]);
    exit:
    return exit_code;
}
