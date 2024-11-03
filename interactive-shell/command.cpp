//
// Created by ruan on 24-11-2.
//

#include "command.h"

Command::Command()
    : Command("", std::vector<std::string>(0))
{
}

Command::Command(std::string command, const std::vector<std::string>& args)
    : command(std::move(command)), args(args), count_args(args.size())
{
    this->input_file = "";
    this->output_file = "";
}

std::string Command::get_command() const
{
    return this->command;
}

std::vector<std::string> Command::get_args() const
{
    return this->args;
}

size_t Command::get_num_of_args() const
{
    return this->count_args;
}

bool Command::exists_input_file() const
{
    return !this->input_file.empty();
}

bool Command::exists_output_file() const
{
    return !this->output_file.empty();
}

std::string Command::get_input_file() const
{
    return this->input_file;
}

std::string Command::get_output_file() const
{
    return this->output_file;
}

void Command::add_arg(const std::string& arg)
{
    args.push_back(arg);
    this->count_args++;
}

void Command::set_command(const std::string& cmd)
{
    this->command = cmd;
}

void Command::set_input_file(const std::string& input_file)
{
    this->input_file = input_file;
}

void Command::set_output_file(const std::string& output_file)
{
    this->output_file = output_file;
}

bool Command::operator==(const Command& other) const
{
    return this->command == other.command && this->args == other.args;
}

std::size_t CommandHash::operator()(const Command& cmd) const
{
    std::size_t hashValue = std::hash<std::string>()(cmd.get_command());
    for (const auto& arg : cmd.get_args())
    {
        hashValue ^= std::hash<std::string>()(arg) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    }
    return hashValue;
}
