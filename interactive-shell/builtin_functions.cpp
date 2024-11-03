//
// Created by ruan on 24-10-24.
//

#include "builtin_functions.h"

#include <csignal>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <sys/wait.h>

#include "main.h"

void builtin_newline()
{
    std::cout << std::endl;
}

template <typename T>
void builtin_echo(T arg)
{
    std::cout << arg;
}

template <typename T, typename... Args>
void builtin_echo(T firstArg, Args... args)
{
    std::cout << firstArg;
    builtin_echo(args...);
}

void builtin_clear()
{
    std::cout << "\033[H\033[2J" << std::flush;
}

void builtin_error(const std::string& error_info)
{
    std::cerr << error_info << std::endl;
}

void builtin_cd(const std::string& target_path)
{
    if (chdir(target_path.c_str()) != 0)
    {
        builtin_error("Unknown directory: " + target_path);
    }
    pi.refreshCwd();
}

void builtin_pwd()
{
    builtin_echo(pi.getPath());
    builtin_newline();
}


void builtin_exit()
{
    builtin_newline();
    builtin_echo("Bye");
    builtin_newline();
    exit(0);
}


std::string exec_builtin_command(const Command& command)
{
    if (command.exists_input_file())
    {
        const int fd = open(command.get_input_file().c_str(), O_RDONLY);
        if (fd == -1)
        {
            perror("failed to open file");
            return "";
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (command.exists_output_file())
    {
        const int fd = open(command.get_output_file().c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("failed to open file");
            return "";
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    std::string cmd = command.get_command();
    if (cmd == "exit")
    {
        // builtin_newline();
        builtin_echo("Bye");
        builtin_newline();
        if (cmd == "exit")
        {
            kill(getppid(), SIGTERM);
            exit(0);
        }
    }

    if (cmd == "echo")
    {
        for (const std::string& arg : command.get_args())
        {
            builtin_echo(arg);
            builtin_echo(" ");
        }
        builtin_newline();
    }
    else if (cmd == "clear")
    {
        builtin_clear();
    }
    else if (cmd == "cd")
    {
        if (command.get_num_of_args() > 1)
        {
            builtin_error("error: too much args");
        }
        else if (command.get_num_of_args() == 1)
        {
            builtin_cd(command.get_args()[0]);
        }
        else
        {
            builtin_error("error: cd requires an argument");
        }
    }
    else if (cmd == "pwd")
    {
        builtin_pwd();
    }

    dup2(ORIGINAL_INPUT, STDIN_FILENO);
    dup2(ORIGINAL_OUTPUT, STDOUT_FILENO);

    return "";
}

void exec_shell_command(const std::vector<Command>& commands)
{
    const size_t count_commands = commands.size();
    if (!count_commands) return;
    std::vector<int> pipe_fds;

    for (int i = 0; i < count_commands - 1; ++i)
    {
        int fd[2];
        if (pipe(fd) == -1)
        {
            builtin_error("Failed to create pipe");
            return;
        }
        pipe_fds.push_back(fd[0]);
        pipe_fds.push_back(fd[1]);
    }

    for (int i = 0; i < count_commands; ++i)
    {
        const pid_t pid = fork();

        if (pid == 0)
        {
            if (commands[i].exists_input_file())
            {
                const int input_fd = open(commands[i].get_input_file().c_str(), O_RDONLY);
                if (input_fd == -1)
                {
                    perror("failed to open file");
                    exit(1);
                }
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            else if (i > 0)
            {
                dup2(pipe_fds[(i - 1) * 2], STDIN_FILENO);
            }

            if (commands[i].exists_output_file())
            {
                const int output_fd = open(commands[i].get_output_file().c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (output_fd == -1)
                {
                    perror("failed to open file");
                    exit(1);
                }
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            else if (i < count_commands - 1)
            {
                dup2(pipe_fds[i * 2 + 1], STDOUT_FILENO);
            }

            for (const int fd : pipe_fds)
            {
                close(fd);
            }

            std::string t_path = commands_path[commands[i]];
            std::vector<std::string> args = commands[i].get_args();
            std::vector<char*> exec_args(args.size() + 2);

            exec_args[0] = const_cast<char*>(t_path.c_str());
            for (size_t j = 0; j < args.size(); ++j)
            {
                exec_args[j + 1] = const_cast<char*>(args[j].c_str());
            }
            exec_args.back() = nullptr;

            execvp(exec_args[0], exec_args.data());

            builtin_error("Execution error: " + commands[i].get_command());
            exit(1);
        }
        else if (pid < 0)
        {
            builtin_error("Fork error for command: " + commands[i].get_command());
            return;
        }
    }

    for (const int fd : pipe_fds)
    {
        close(fd);
    }

    for (int i = 0; i < count_commands; ++i)
    {
        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            // std::cout << "Command exited with status: " << WEXITSTATUS(status) << std::endl;
        }
    }

    dup2(ORIGINAL_INPUT, STDIN_FILENO);
    dup2(ORIGINAL_OUTPUT, STDOUT_FILENO);
}


bool find_builtin_function(const Command& command)
{
    const std::string cmd = command.get_command();
    return cmd == "pwd" || cmd == "cd" || cmd == "echo" || cmd == "exit" || cmd == "clear";
}

bool find_shell_function(const Command& command)
{
    if (commands_path.contains(command)) return true;
    const std::vector<std::string> paths = pi.getDefaultPaths();
    struct stat buffer{};

    for (const std::string& path : paths)
    {
        std::string t_path = path + "/" + command.get_command();
        if (stat(t_path.c_str(), &buffer)) continue;
        if (buffer.st_mode & S_IFREG && access(t_path.c_str(), X_OK) == 0)
        {
            commands_path[command] = t_path;
            return true;
        }
    }
    return false;
}
