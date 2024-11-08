//
// Created by ruan on 24-11-7.
//

#ifndef MAIN_H
#define MAIN_H
#include <sys/ioctl.h>

inline bool read_from_stdin = false;

inline struct winsize ws;

inline void init();

void exec_less(const char *path);

#endif //MAIN_H
