#include "index.h"
#include "ninja.h"
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);

    auto index = Index{};

    writeNinjaFile(index);

    auto cachePath = "build/.mm3/default";

    return std::system(
        ("ninja -f " +
         (std::filesystem::path{cachePath} / "build.ninja").string())
            .c_str());
}
