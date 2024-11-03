#include <iostream>

#include "main.h"

#include <cstring>

void exec_mv(const char* source, const char* dest)
{
    if(strcmp(dest, source) == 0)
    {
        return;
    }

    FILE* src = fopen(source, "rb");
    if(!src)
    {
        std::cerr << "Error opening file " << source << std::endl;
        exit_code = 1;
        return;
    }

    FILE* dst = fopen(dest, "wb");
    if(!dst)
    {
        std::cerr << "Error opening file " << dest << std::endl;
        fclose(src);
        exit_code = 1;
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, src)) > 0) fwrite(buffer, sizeof(char), bytes_read, dst);

    fclose(src);
    fclose(dst);

    if(remove(source))
    {
        std::cerr << "Error removing file " << source << std::endl;
        exit_code = 1;
    }
}
int main(const int argc, char **argv)
{
    if(argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " SOURCE DEST" << std::endl;
        exit(1);
    }

    exec_mv(argv[1], argv[2]);

    return exit_code;
}

