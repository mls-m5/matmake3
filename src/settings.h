#pragma once

#include "buildpaths.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

struct Settings {

    enum Command {
        Build,
        Clean,
    };

    std::string linkFlags;
    std::string cxxflags;
    BuildPaths paths;
    bool shouldBuildNative = false;
    int numCores = -1;
    bool isVerbose = false;
    bool shouldOutputCompileCommands = false;

    Command command = Build;

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
            else if (arg == "--verbose" || arg == "-v") {
                isVerbose = true;
            }
            else if (arg == "--clean") {
                command = Clean;
            }
            else if (arg == "--compile-commands") {
                shouldOutputCompileCommands = true;
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
matmake3

args

--help -h                 print this text
--link                    extra link flags
--debug                   add debug flags to build
--flag                    add build flags
-j                        number of process to use when building
--file -f                 specify path to .mm3 file to use
--verbose -v              print information of matmakes process
--clean                   remove all built files
--compile-commands        print clang compile commands


)_";
        std::exit(returnCode);
    }
};
