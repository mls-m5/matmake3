#include "index.h"
#include "ninja.h"
#include <iostream>

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);

    auto fileIndex = std::make_shared<Index>();

    //    runLua(*fileIndex);

    writeNinjaFile(*fileIndex);

    auto cachePath = "build/.mm3/default";

    return std::system(
        ("ninja -f " +
         (std::filesystem::path{cachePath} / "build.ninja").string())
            .c_str());
}
