#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "main.h"

#include <cstring>

void exec_ls(const std::string& path)
{
    DIR* dir = opendir(path.c_str());
    if (!dir)
    {
        std::cerr << "Failed to open directory " << path << std::endl;
        exit_code = 1;
        return;
    }

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            if (entry->d_name[0] == '.') continue;

            struct stat file_stat{};
            std::string full_path = path + "/" + entry->d_name;

            if (lstat(full_path.c_str(), &file_stat) == 0)
            {
                if (S_ISDIR(file_stat.st_mode)) // directory
                {
                    std::cout << "\033[1;34m" << entry->d_name << "\033[0m/ ";
                }
                else if (S_ISLNK(file_stat.st_mode)) // link file
                {
                    std::cout << "\033[1;36m" << entry->d_name << "\033[0m@ ";
                }
                else if (file_stat.st_mode & S_IXUSR) // executable file
                {
                    std::cout << "\033[1;32m" << entry->d_name << "\033[0m* ";
                }
                else
                {
                    std::cout << entry->d_name << " ";
                }
            }
            else
            {
                std::cerr << "file_stat error" << std::endl;
                exit_code = 1;
            }
        }
    }
    std::cout << std::endl;
    closedir(dir);
}


int main(int argc, char** argv)
{
    std::string path = ".";

    if (argc > 1) path = argv[1];

    exec_ls(path);

exit:
    return exit_code;
}
