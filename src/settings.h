#pragma once

#include "buildpaths.h"
#include <iostream>
#include <string>
#include <vector>

struct Settings {
    std::string linkFlags;
    BuildPaths paths;

    Settings(int argc, char **argv) {
        auto args = std::vector<std::string>(argv + 1, argv + argc);

        for (size_t i = 0; i < args.size(); ++i) {
            auto arg = args.at(i);

            if (arg.front() != '-') {
                std::cerr << "invalid argument " << arg << "\n\n";
                printHelp(1);
            }
            else if (arg == "--help" || arg == "-h") {
                printHelp();
            }
            else if (arg == "--link") {
                linkFlags += " " + args.at(++i);
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


)_";
        std::exit(returnCode);
    }
};
