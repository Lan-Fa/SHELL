//
// Created by ruan on 24-11-7.
//

#ifndef MAIN_H
#define MAIN_H
#include <vector>
#include <sys/ioctl.h>

inline bool read_from_stdin = false;

inline struct winsize ws;

inline void init();

void display(int l, int r, const std::vector<std::string>& file_content);

void exec_less(std::istream& input_stream);

#endif //MAIN_H
