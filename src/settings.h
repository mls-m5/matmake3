#pragma once

#include "buildpaths.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

struct Settings {
    std::string linkFlags;
    std::string cxxflags;
    BuildPaths paths;
    bool shouldBuildNative = false;
    int numCores = -1;

    Settings(int argc, char **argv) {
        auto args = std::vector<std::string>(argv + 1, argv + argc);

        for (size_t i = 0; i < args.size(); ++i) {
            auto arg = args.at(i);

            if (arg.front() != '-') {
                if (!std::filesystem::is_directory(arg)) {
                    std::cerr << "invalid argument " << arg << "\n\n";
                    printHelp(1);
                }
                std::filesystem::current_path(arg);
            }
            else if (arg == "--help" || arg == "-h") {
                printHelp();
            }
            else if (arg == "--link") {
                linkFlags += " " + args.at(++i);
            }
            else if (arg == "-j") {
                numCores = std::stoi(args.at(++i));
            }
            else if (arg == "--debug") {
                cxxflags += " -g";
            }
            else if (arg == "--flag") {
                cxxflags += " " + args.at(++i);
            }
            else if (arg == "--file" || arg == "-f") {
                paths.buildScript = args.at(++i);
            }
            else {
                std::cerr << "invalid argument " << arg << "\n\n";
                printHelp(1);
            }
        }
    }

    void printHelp(int returnCode = 0) {
        std::cout << R"_(
usage:
matmake3  [--in infile] [--out outfile]

args

--help -h                 print this text
--link                    extra link flags
--debug                   add debug flags to build
--flag                    add build flags
-j                        number of process to use when building


)_";
        std::exit(returnCode);
    }
};
