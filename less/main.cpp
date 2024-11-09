#include <cstring>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "main.h"

#include <fstream>
#include <vector>

void init(const int fd)
{
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
    {
        std::cerr << "Error getting terminal size." << std::endl;
        return;
    }

    struct termios term;
    tcgetattr(fd, &term); // 获取终端设置
    term.c_lflag &= ~(ICANON | ECHO); // 关闭行缓冲和回显
    term.c_cc[VMIN] = 1; // 设置最小输入字节数为1
    term.c_cc[VTIME] = 0; // 不等待任何时间
    tcsetattr(fd, TCSANOW, &term); // 立即应用设置
}

void reset(const int fd)
{
    struct termios term;
    tcgetattr(fd, &term);
    term.c_lflag |= (ICANON | ECHO); // 恢复行缓冲和回显
    tcsetattr(fd, TCSANOW, &term);
}

void display(int l, int r, const std::vector<std::string>& file_content)
{
    for(int i = l; i < r; i++) std::cout << file_content[i] << std::endl;
    for(int i = r - l; i < ws.ws_row - 1; i++) std::cout << std::endl;
}

void exec_less(std::istream& file, const int tty_fd)
{
    init(tty_fd == -1 ? STDIN_FILENO : tty_fd);
    std::vector<std::string> file_content;
    for(int i = 0; i < ws.ws_row - 1; i++)
    {
        std::string buf;
        if(!std::getline(file, buf)) break;;
        file_content.push_back(buf);
    }
    int l = 0, r = std::min(static_cast<int>(ws.ws_row), static_cast<int>(file_content.size()));
    display(l, r, file_content);
    char op;
    std::string buf;
    unsigned max_row = 0x7FFFFFFF;
    const int pipe_fd = dup(STDIN_FILENO);
    while (true)
    {
        std::cout << (r == max_row ? "\033[7m(END)\033[0m" : ":");
        if(tty_fd != -1)
        {
            dup2(tty_fd, STDIN_FILENO);
        }
        if(!(std::cin >> op)) break;
        if(op == 'q')
        {
            break;
        }
        std::cout << std::endl;
        switch(op)
        {
        case 'k':
            if(l > 0)
            {
                l = std::max(l - 1, 0);
                r = std::max(r - 1, static_cast<int>(ws.ws_row) - 1);
            }
            break;
        case 'j':
            dup2(pipe_fd, STDIN_FILENO);
            if(std::getline(file, buf)) file_content.push_back(buf);
            else max_row = file_content.size();
            l = std::min(l + 1, static_cast<int>(file_content.size() - ws.ws_row) + 1);
            r = std::min(r + 1, static_cast<int>(file_content.size()));
            break;
        case 'g':
            l = 0, r = std::min(static_cast<int>(ws.ws_row) - 1, static_cast<int>(file_content.size()));
            break;
        default:
            break;
        }
        display(l, r, file_content);
    }
    reset(tty_fd == -1 ? STDIN_FILENO : tty_fd);
}


int main(const int argc, const char** argv)
{
    if (isatty(STDIN_FILENO)) {
        std::cout << "Input is from terminal" << std::endl;
        std::ifstream file;
        file.open(argv[1]);
        if(!file.is_open())
        {
            std::cerr << "Error opening file." << std::endl;
        } else
        {
            exec_less(file, -1);
        }
        file.close();
    } else {
        // std::cout << "Input is from pipe or file" << std::endl;
        const int tty_fd = open("/dev/tty", O_RDWR);
        if(tty_fd == -1)
        {
            perror("open");
            return 1;
        }
        exec_less(std::cin, tty_fd);
    }
    return 0;
}
