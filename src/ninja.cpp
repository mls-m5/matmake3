#include "ninja.h"
#include "buildpaths.h"
#include "log.h"
#include "settings.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {

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

auto ninjaPath(const BuildPaths &paths) {
    return std::filesystem::path{std::filesystem::path{paths.cache} /
                                 "build.ninja"};
}

} // namespace

void writeNinjaFile(const BuildPaths &paths, const CommandList &list) {
    std::filesystem::create_directories(paths.cache);
    auto ninjaPath = ::ninjaPath(paths);
    {
        auto file = std::ofstream{ninjaPath};

        if (!file) {
            throw std::runtime_error{"could not open " + ninjaPath.string() +
                                     " for writing"};
        }

        mlog.debug("writing to", std::filesystem::absolute(ninjaPath));

        file << "# Ninja file generated with matmake3\n\n";
        file << "rule run\n";
        file << "    description = $dsc\n";
        file << "    command = $cmd\n\n";
        file << "rule run_dep\n";
        file << "    description = $dsc\n";
        file << "    command = $cmd\n";
        file << "    depfile = $out.d\n\n";

        for (auto &command : list.commands()) {
            std::string runCommand = "run";
            if (command.file.shouldUseDepFile) {
                runCommand = "run_dep";
            }
            file << "build " << command.out.string() << ": " << runCommand
                 << " ";

            if (command.file.src) {
                file << " " << command.file.src->fullPath.string();
            }
            for (auto dep : command.file.dependencies) {
                file << " " << dep->fullPath.string();
            }
            file << "\n  dsc = " << command.file.path.filename().string();
            file << "\n  cmd = " << command.command << "\n\n";
        }
    }
}

void runNinjaFile(const Settings &settings) {
    auto ss = std::ostringstream{};
    ss << "NINJA_STATUS=\"_\"; ninja -f " << ninjaPath(settings.paths) << " "
       << ((settings.numCores != -1)
               ? ("-j " + std::to_string(settings.numCores))
               : std::string{})
       << " 2>&1";
    if (std::system(ss.str().c_str())) {
        throw std::runtime_error{"ninja build failed"};
    }
}

void ninjaClean(const BuildPaths &paths) {
    auto ninjaPath = ::ninjaPath(paths);

    std::system(("ninja -f \"" + ninjaPath.string() + "\" -t clean").c_str());
}
