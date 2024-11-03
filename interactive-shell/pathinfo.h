//
// Created by ruan on 24-10-24.
//

#ifndef PATH_INFO_H
#define PATH_INFO_H
#include <string>
#include <vector>
#include <linux/limits.h>

class PathInfo
{
public:
    PathInfo();

    void refreshCwd();
    [[nodiscard]] char* getPath();
    [[nodiscard]] std::vector<std::string> getDefaultPaths();
    void setPath(const char* path);

private:
    char cwd[PATH_MAX]{};
    std::vector<std::string> default_paths;
};

#endif //PATH_INFO_H
