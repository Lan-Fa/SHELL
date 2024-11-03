//
// Created by ruan on 24-10-24.
//

#ifndef INNER_FUNCTIONS_H
#define INNER_FUNCTIONS_H

#include "main.h"

template <typename T>
void builtin_echo(T arg);

template <typename T, typename... Args>
void builtin_echo(T firstArg, Args... args);

void builtin_newline();

void builtin_clear();

void builtin_error(const std::string& error_info);

void builtin_cd(const std::string& target_path);

void builtin_pwd();

void builtin_exit();

bool find_builtin_function(const Command& command);

bool find_shell_function(const Command& command);

std::string exec_builtin_command(const Command& command);

void exec_shell_command(const std::vector<Command>& commands);
#endif //INNER_FUNCTIONS_H
