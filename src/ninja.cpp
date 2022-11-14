#include "ninja.h"
#include "buildpaths.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

void printEscaped(std::ostream &stream, std::string_view str) {
    for (auto c : str) {
        if (c == ' ') {
            stream << "$ ";
        }
        else {
            stream << c;
        }
    }
}

void writeNinjaFile(const BuildPaths &paths, const CommandList &list) {

    std::filesystem::create_directories(paths.cache);
    auto ninjaPath = std::filesystem::path{std::filesystem::path{paths.cache} /
                                           "build.ninja"};
    {
        auto file = std::ofstream{ninjaPath};

        if (!file) {
            throw std::runtime_error{"could not open " + ninjaPath.string() +
                                     " for writing"};
        }

        std::cout << "writing to " << std::filesystem::absolute(ninjaPath)
                  << std::endl;

        file << "# Ninja file generated with matmake3\n\n";
        file << "rule run\n";
        file << "    command = $cmd\n";
        file << "  depfile = $out.d\n\n";

        for (auto &command : list.commands()) {
            file << "build " << command.out.string() << ": run";

            if (command.file.src) {
                file << " " << command.file.src->fullPath.string();
            }
            for (auto dep : command.file.dependencies) {
                file << " " << dep->fullPath.string();
            }
            file << "\n  cmd = " << command.command << "\n\n";
        }
    }

    {
        auto ss = std::ostringstream{};
        ss << "ninja -f " << ninjaPath;
        if (std::system(ss.str().c_str())) {
            throw std::runtime_error{"ninja build failed"};
        }
    }
}
