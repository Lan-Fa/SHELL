#include "main.h"

#include <csignal>
#include <cstring>
#include <iostream>

void exec_kill(const pid_t pid, const int sig)
{
    if(pid == 0) return;
    exit_code = kill(pid, 9) == -1 ? 1 : 0;
    if(exit_code == 1)
    {
        perror("kill");
    }
}


int main(const int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " [-<signal>] <pid>" << std::endl;
        exit_code = 1;
        return exit_code;
    }

    if(argc == 2)
    {
        const pid_t pid = std::stoi(argv[1]);
        exec_kill(pid, SIGTERM);
        std::cout << pid << std::endl;
    } else
    {
        char buf[50];
        strncpy(buf, argv[1] + 1, strlen(argv[1]));
        const pid_t pid = std::stoi(argv[2]);
        const int sig = std::stoi(buf);
        exec_kill(pid, sig);
    }

    return exit_code;
}
