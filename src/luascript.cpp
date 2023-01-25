#include "luascript.h"
#include "sol/sol.hpp"
#include <filesystem>

bool hasBuildScript(const BuildPaths &paths) {
    return std::filesystem::exists(paths.buildScript);
}

void runBuildScript(const BuildPaths &paths,
                    const Index &index,
                    Target &target) {}
