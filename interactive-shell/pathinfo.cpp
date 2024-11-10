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
    this->env["PATH"] = d_path;
    d_path = ":/usr/bin";
    this->env["PATH"].append(d_path);
}

void PathInfo::refreshCwd()
{
    if (getcwd(this->cwd, sizeof(this->cwd)) == nullptr)
    {
        std::cerr << "Get current path error" << std::endl;
    }
}

char* PathInfo::getPath()
{
    return this->cwd;
}

void PathInfo::setPath(const char* path)
{
    strcpy(this->cwd, path);
}

std::pair<std::string, bool> PathInfo::getEnv(const std::string& key)
{
    return this->env.contains(key) ?
        std::make_pair(env[key], true) : std::make_pair("", false);
}

void PathInfo::addEnv(const std::string& key, const std::string& value)
{
    this->env[key] = value;
}

