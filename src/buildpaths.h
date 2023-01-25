#pragma once

#include <filesystem>

struct BuildPaths {
    std::filesystem::path cache = "build/.mm3/default";
    std::filesystem::path out = "build/default";
    std::filesystem::path buildScript = "build.mm3";
};
