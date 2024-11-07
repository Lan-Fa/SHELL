//
// Created by ruan on 24-10-23.
//

#ifndef MAIN_H
#define MAIN_H
#include <unordered_map>
#include <vector>
#include <unistd.h>

#include "command.h"
#include "pathinfo.h"

#define TARGET_PATH args[0]
#define COUNT_ARGS args.size()


inline bool skip = false;

inline PathInfo pi;

const int ORIGINAL_INPUT = dup(STDIN_FILENO);
const int ORIGINAL_OUTPUT = dup(STDOUT_FILENO);

inline const char* share_memory_name;

inline constexpr size_t share_memory_size = sizeof(PathInfo);

inline std::vector<std::string> commands;

inline std::unordered_map<Command, std::string, CommandHash> commands_path;

void SIGINT_Handler(int signum);

void init();

void exec_command(const std::vector<Command>& commands);

#endif //MAIN_H
