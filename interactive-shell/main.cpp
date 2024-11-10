#include <iostream>
#include <string>
#include <csignal>
#include <sstream>
#include <vector>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "main.h"
#include "builtin_functions.h"

void SIGINT_Handler(const int signum)
{
    if (signum == SIGINT)
    {
        skip = true;
    }
}

void init()
{
    builtin_clear();
    signal(SIGINT, SIGINT_Handler);
}

void exec_command(const std::vector<Command>& commands)
{
    std::vector<Command> shell_commands;
    std::vector<Command> builtin_commands;

    for (const Command& command : commands)
    {
        if (find_shell_function(command)) shell_commands.push_back(command);
        else if (find_builtin_function(command)) builtin_commands.push_back(command);
        else
        {
            builtin_error("Unknown command: " + command.get_command());
        }
    }

    for (const Command& command : builtin_commands) exec_builtin_command(command);

    exec_shell_command(shell_commands);
}

int
main()
{
    init();

    while (true)
    {
        std::string input;
        builtin_echo(pi.getPath());
        input = readline("> ");

        if (std::cin.eof())
        {
            builtin_newline();
            builtin_echo("Bye");
            builtin_newline();
            exit(0);
        }

        if (skip)
        {
            skip = false;
            continue;
        }

        std::istringstream iss(input);
        std::vector<Command> commands;
        std::string tmp_command;

        while (getline(iss, tmp_command, '|'))
        {
            std::istringstream parse_command_stream(tmp_command);
            Command cmd;
            std::string tmp_str;
            parse_command_stream >> tmp_str;
            cmd.set_command(tmp_str);
            while (parse_command_stream >> tmp_str)
            {
                if(tmp_str == "<")
                {
                    std::string input_file;
                    parse_command_stream >> input_file;
                    cmd.set_input_file(input_file);
                } else if(tmp_str == ">")
                {
                    std::string output_file;
                    parse_command_stream >> output_file;
                    cmd.set_output_file(output_file);
                } else
                {
                    if(tmp_str[0] == '$')
                    {
                        tmp_str = pi.getEnv(tmp_str.substr(1)).first;
                    }
                    cmd.add_arg(tmp_str);
                }
            }
            commands.push_back(cmd);
        }

        exec_command(commands);
    }

    return 0;
}
