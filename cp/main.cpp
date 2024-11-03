#include <iostream>

#include "main.h"

void exec_cp(const char* source, const char* target)
{
    FILE* src = fopen(source, "rb");
    FILE* dst = fopen(target, "wb");

    if(!src)
    {
        std::cerr << "Error opening file " << source << std::endl;
        exit_code = 1;
        fclose(dst);
        return;
    } else if(!dst)
    {
        std::cerr << "Error opening file " << target << std::endl;
        exit_code = 1;
        fclose(src);
        return;
    } else
    {
        char buffer[BUFFER_SIZE];

        size_t bytes_read;
        while((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, src)) > 0) fwrite(buffer, sizeof(char), bytes_read, dst);

        fclose(src);
        fclose(dst);
    }
}

int main(const int argc, char **argv)
{
    if(argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
        exit(1);
    }

    exec_cp(argv[1], argv[2]);

    return exit_code;
}

