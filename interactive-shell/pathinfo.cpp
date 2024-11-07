//
// Created by ruan on 24-10-24.
//

#include "pathinfo.h"

#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>

PathInfo::PathInfo()
{
    refreshCwd();
    std::string d_path = "/home/ruan/develop/SHELL/SHELL_PATH";
    PATH.emplace_back(d_path.c_str());
    d_path = "/usr/bin";
    PATH.emplace_back(d_path.c_str());
}

void PathInfo::refreshCwd()
{
    if (getcwd(cwd, sizeof(cwd)) == nullptr)
    {
        std::cerr << "Get current path error" << std::endl;
    }
}

char* PathInfo::getPath()
{
    return cwd;
}

void PathInfo::setPath(const char* path)
{
    strcpy(cwd, path);
}

std::vector<std::string> PathInfo::getDefaultPaths()
{
    return this->PATH;
}
