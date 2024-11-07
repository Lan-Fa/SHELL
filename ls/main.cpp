#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <clocale>

#include "main.h"

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <map>
#include <vector>

std::string change_to_human_read(size_t size)
{
    // TODO: complete the func
    return "";
}

void exec_ls(const std::string& path)
{
    setlocale(LC_TIME, "");

    DIR* dir = opendir(path.c_str());
    if (!dir)
    {
        std::cerr << "Failed to open directory " << path << std::endl;
        exit_code = 1;
        return;
    }

    std::vector<std::pair<dirent*, struct stat>> entries(0);

    int max_link_len = -INF;
    int max_owner_len = -INF;
    int max_group_len = -INF;
    int max_size_len = -INF;

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") == 0) continue;
        if (entry->d_name[0] == '.') continue;

        struct stat file_stat{};
        std::string full_path = path + "/" + entry->d_name;

        if (stat(full_path.c_str(), &file_stat) != 0)
        {
            std::cerr << "file_stat error" << std::endl;
            exit_code = 1;
            return;
        }

        entries.emplace_back(entry, file_stat);

        const struct passwd* pw = getpwuid(file_stat.st_uid);
        const struct group* gr = getgrgid(file_stat.st_gid);
        max_link_len = std::max(max_link_len, static_cast<int>(std::to_string(file_stat.st_nlink).length()));
        max_owner_len = std::max(max_owner_len, pw ? static_cast<int>(strlen(pw->pw_name)) : 0);
        max_group_len = std::max(max_group_len, gr ? static_cast<int>(strlen(gr->gr_name)) : 0);
        max_size_len = std::max(max_size_len, static_cast<int>(std::to_string(file_stat.st_size).length()));
    }

    delete(entry);

    std::ranges::sort(entries, [&](const auto& e1, const auto& e2)
    {
        return strcasecmp(e1.first->d_name, e2.first->d_name) < 0;
    });

    bool enter_flag = false;

    for (auto [entry, file_stat] : entries)
    {
        if (enter_flag && long_listing_format) std::cout << std::endl;
        enter_flag = true;

        if (long_listing_format)
        {
            std::cout << (S_ISDIR(file_stat.st_mode) ? 'd' : '-');
            std::cout << ((file_stat.st_mode & S_IRUSR) ? 'r' : '-');
            std::cout << ((file_stat.st_mode & S_IWUSR) ? 'w' : '-');
            std::cout << ((file_stat.st_mode & S_IXUSR) ? 'x' : '-');
            std::cout << ((file_stat.st_mode & S_IRGRP) ? 'r' : '-');
            std::cout << ((file_stat.st_mode & S_IWGRP) ? 'w' : '-');
            std::cout << ((file_stat.st_mode & S_IXGRP) ? 'x' : '-');
            std::cout << ((file_stat.st_mode & S_IROTH) ? 'r' : '-');
            std::cout << ((file_stat.st_mode & S_IWOTH) ? 'w' : '-');
            std::cout << ((file_stat.st_mode & S_IXOTH) ? 'x' : '-') << " ";

            // Number of hard links
            std::cout << std::setw(max_link_len) << file_stat.st_nlink << " ";

            // Owner and Group
            const struct passwd* pw = getpwuid(file_stat.st_uid);
            const struct group* gr = getgrgid(file_stat.st_gid);
            std::cout << std::setw(max_owner_len) << (pw ? pw->pw_name : "") << " ";
            std::cout << std::setw(max_group_len) << (gr ? gr->gr_name : "") << " ";

            // File size
            std::cout << std::setw(max_size_len) << file_stat.st_size << " ";

            // Last modified time
            char time_buffer[100];
            const struct tm* tm_info = localtime(&file_stat.st_mtime);
            strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", tm_info);
            std::cout << time_buffer << " ";
        }

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

    std::cout << std::endl;
    closedir(dir);
}

int main(const int argc, char** argv)
{
    std::string path = ".";

    if (argc == 2)
    {
        if (strcmp("-l", argv[1]) == 0) long_listing_format = true;
        else path = argv[1];
    }
    else if (argc == 3)
    {
        path = argv[2];
        if (strcmp("-l", argv[1]) == 0) long_listing_format = true;
    }

    exec_ls(path);

exit:
    return exit_code;
}
