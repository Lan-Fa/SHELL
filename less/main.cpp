#include <cstring>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#include "main.h"

void init()
{
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
    {
        std::cerr << "Error getting terminal size." << std::endl;
        return;
    }

    struct termios term;
    tcgetattr(STDIN_FILENO, &term); // 获取终端设置
    term.c_lflag &= ~(ICANON | ECHO); // 关闭行缓冲和回显
    term.c_cc[VMIN] = 1; // 设置最小输入字节数为1
    term.c_cc[VTIME] = 0; // 不等待任何时间
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // 立即应用设置
}

void reset_terminal_mode()
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO); // 恢复行缓冲和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void exec_less(const char* path)
{
    init();
    if (!read_from_stdin) freopen(path, "r", stdin);
    char c;
    while (std::cin >> c)
    {
        if (c == 'q') break;
        std::cout << c << std::endl;
    }
    reset_terminal_mode();
}


int main(const int argc, const char** argv)
{
    if (isatty(STDIN_FILENO)) {
        std::cout << "Input is from terminal" << std::endl;
    } else {
        std::cout << "Input is from pipe or file" << std::endl;
    }
    return 0;
    if (argc == 2 && strcmp(argv[1], "-stdin") == 0)
    {
        read_from_stdin = true;
        exec_less("");
    }
    else if (argc == 2)
    {
        exec_less(argv[1]);
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
        return 1;
    }
    return 0;
}


// #include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>
//
// void display_file_page(std::ifstream &file, std::streampos &pos, size_t page_size) {
//     // 设置文件指针
//     file.seekg(pos);
//
//     // 缓冲区存储当前页的内容
//     std::vector<std::string> page_lines;
//     std::string line;
//     size_t lines_read = 0;
//
//     // 读取当前页的内容
//     while (lines_read < page_size && std::getline(file, line)) {
//         page_lines.push_back(line);
//         lines_read++;
//     }
//
//     // 显示当前页内容
//     for (const auto& ln : page_lines) {
//         std::cout << ln << std::endl;
//     }
//
//     // 更新文件指针位置
//     pos = file.tellg();
// }
//
// int main(int argc, char **argv) {
//     std::ifstream file(argv[1]);
//     if (!file.is_open()) {
//         std::cerr << "Error opening file!" << std::endl;
//         return 1;
//     }
//
//     size_t page_size = 10; // 每次显示20行
//     std::streampos current_pos = 0;
//
//     // 初始读取第一页
//     display_file_page(file, current_pos, page_size);
//
//     // 模拟分页，等待用户输入
//     char cmd;
//     while (true) {
//         std::cout << "Press 'n' for next page, 'q' to quit: ";
//         std::cin >> cmd;
//
//         if (cmd == 'n') {
//             // 显示下一页
//             display_file_page(file, current_pos, page_size);
//         } else if (cmd == 'q') {
//             break;
//         }
//     }
//
//     file.close();
//     return 0;
// }
