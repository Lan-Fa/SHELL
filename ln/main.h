//
// Created by ruan on 24-11-2.
//

#ifndef MAIN_H
#define MAIN_H

inline int exit_code = 0;

void create_hard_link(const char* TARGET, const char* LINK_NAME);

void create_symbolic_link(const char* TARGET, const char* LINK_NAME);

#endif //MAIN_H
