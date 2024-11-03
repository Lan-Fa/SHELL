#include <iostream>

#include "main.h"

#include <cstring>

void exec_cat(const char* filename)
{
    FILE* file;

    if(strcmp(filename, "-") == 0)
    {
        file = stdin;
    } else
    {
        file = fopen(filename, "rb");
        if(!file)
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
            exit_code = 1;
            return;
        }
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
    {
        fwrite(buffer, 1, bytes_read, stdout);
    }

    if (file != stdin) {
        fclose(file);
    }
}


int main(int argc, char **argv)
{
    if(argc < 2)
    {
        // std::cerr << "Usage: " << argv[0] << "<file1> <file2> ..." << std::endl;
        // exit_code = 1;
        // goto exit;
        exec_cat("-");
        return 0;
    }

    for(int i = 1; i < argc; i++)
    {
        exec_cat(argv[i]);
        if(exit_code) goto exit;
    }

    exit:
    return exit_code;
}
