//
// Created by ruan on 24-11-7.
//

#ifndef MAIN_H
#define MAIN_H
#include <vector>
#include <sys/ioctl.h>

#define OP_Q 0
#define OP_J 1
#define OP_K 2
#define OP_G 3

#define TERM_INPUT 0
#define TERM_FILE 1

inline bool read_from_stdin = false;

inline struct winsize ws;

void init(int fd, int type);

void reset(int fd);

void display(int l, int r, const std::vector<std::string>& file_content);

void exec_less(std::istream& file, int tty_fd);

#endif //MAIN_H
