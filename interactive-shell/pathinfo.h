//
// Created by ruan on 24-10-24.
//

#ifndef PATH_INFO_H
#define PATH_INFO_H
#include <string>
#include <unordered_map>
#include <linux/limits.h>

class PathInfo
{
public:
    PathInfo();

    void refreshCwd();
    [[nodiscard]] char* getPath();
    [[nodiscard]] std::pair<std::string, bool> getEnv(const std::string& key);
    void addEnv(const std::string& key, const std::string& value);
    void setPath(const char* path);

private:
    char cwd[PATH_MAX]{};
    std::unordered_map<std::string, std::string> env;
};

#endif //PATH_INFO_H
