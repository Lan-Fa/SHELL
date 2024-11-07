//
// Created by ruan on 24-11-2.
//

#ifndef MAIN_H
#define MAIN_H
#include <string>

#define INF 0x7FFFFFFF

inline int exit_code = 0;

inline bool long_listing_format = false;

std::string change_to_human_read(size_t size);

void exec_ls(const std::string& path);

#endif //MAIN_H
