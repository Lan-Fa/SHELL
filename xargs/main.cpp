#include "main.h"

#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <cstring>

void exec_command(const std::string& command, const std::vector<std::string>& args)
{
    const pid_t pid = fork();
    if(pid == 0)
    {
        std::vector<char*> exec_args;
        exec_args.push_back(const_cast<char*>(command.c_str()));
        for (const auto& arg : args) {
            exec_args.push_back(const_cast<char*>(arg.c_str()));
        }
        exec_args.push_back(nullptr);

        execvp(exec_args[0], exec_args.data());
        std::cerr << "Error executing command: " << command << std::endl;
        exit(1);
    } else if(pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        if(WIFEXITED(status))
        {
            exit_code = WEXITSTATUS(status);
        }
    }
}


int main(const int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <command> [args...]" << std::endl;
        exit(1);
    }

    std::string command;
    char delimiter = 0;
    if(argc == 4 && strcmp(argv[1], "-d") == 0)
    {
        delimiter = argv[2][0];
        command = argv[3];
    } else
    {
        command = argv[1];
    }

    std::vector<std::string> args;
    std::string tmp_arg;

    if(delimiter == 0)
    {
        while(std::cin >> tmp_arg) args.push_back(tmp_arg);
    } else
    {
        while(std::getline(std::cin, tmp_arg, delimiter)) args.push_back(tmp_arg);
    }

    exec_command(command, args);

    exit:
    return exit_code;
}
