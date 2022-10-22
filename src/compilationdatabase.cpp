#include "compilationdatabase.h"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct DatabaseEntry {
    std::string file;
    std::string directory;
    std::string command;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DatabaseEntry, file, directory, command);

} // namespace

void writeCompilationDatabase(const BuildPaths &paths,
                              const CommandList &list) {
    std::vector<DatabaseEntry> entries;

    for (auto &command : list.commands()) {
        if (command.file.src) {
            entries.push_back(DatabaseEntry{
                .file = std::filesystem::absolute(command.file.src->fullPath)
                            .string(),
                .directory =
                    std::filesystem::absolute(std::filesystem::current_path())
                        .string(),
                .command = command.command,
            });
        }
    }

    auto json = nlohmann::json{entries};

    auto jsonPath = std::filesystem::path{paths.out / "compile_commands.json"};

    std::cout << "writing to file " << jsonPath << std::endl;

    {
        auto file = std::ofstream{jsonPath};
        file << std::setw(2) << json;
    }
}
