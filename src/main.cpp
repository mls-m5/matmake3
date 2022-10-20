#include "createrecursive.h"
#include "index.h"
#include "ninja.h"
#include "target.h"
#include <iostream>

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);

    auto index = std::make_unique<Index>();
    auto target = createRecursive(*index);

    //    writeNinjaFile(*index);

    auto cachePath = "build/.mm3/default";

    return std::system(
        ("ninja -f " +
         (std::filesystem::path{cachePath} / "build.ninja").string())
            .c_str());
}
