#pragma once

#include "file.h"
#include <filesystem>
#include <string>
#include <vector>

struct CommandList {
    struct Command {
        std::string command;
        const File &file;
        std::filesystem::path out;
    };

    void add(std::string command,
             const File &file,
             std::filesystem::path path) {
        _commands.push_back({std::move(command), file, path});
    }

    auto &commands() const {
        return _commands;
    }

    std::filesystem::path buildPath;
    std::filesystem::path projectPath;

private:
    std::vector<Command> _commands;
};
