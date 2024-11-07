//
// Created by ruan on 24-11-7.
//

#ifndef MAIN_H
#define MAIN_H
#include <csignal>

void exec_kill(pid_t pid, int sig);

inline int static exit_code = 0;

#endif //MAIN_H
