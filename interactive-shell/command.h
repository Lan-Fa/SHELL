//
// Created by ruan on 24-11-2.
//

#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <vector>


class Command
{
public:
    Command();

    Command(std::string command, const std::vector<std::string>& args);

    [[nodiscard]] std::string get_command() const;
    [[nodiscard]] std::vector<std::string> get_args() const;
    [[nodiscard]] size_t get_num_of_args() const;
    [[nodiscard]] bool exists_input_file() const;
    [[nodiscard]] bool exists_output_file() const;
    [[nodiscard]] std::string get_input_file() const;
    [[nodiscard]] std::string get_output_file() const;
    void add_arg(const std::string& arg);
    void set_command(const std::string& command);
    void set_input_file(const std::string& input_file);
    void set_output_file(const std::string& output_file);

    bool operator==(const Command& other) const;

private:
    std::string command;
    std::vector<std::string> args;
    size_t count_args = 0;
    std::string input_file;
    std::string output_file;
};

// 自定义哈希函数
struct CommandHash
{
    std::size_t operator()(const Command& cmd) const;
};


#endif //COMMAND_H
