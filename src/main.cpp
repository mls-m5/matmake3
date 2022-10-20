#include "build.h"
#include "createrecursive.h"
#include "index.h"
#include "ninja.h"
#include <iostream>

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);

    auto index = std::make_unique<Index>();
    auto target = createRecursive(*index);

    build(*target);

    auto cachePath = "build/.mm3/default";

    //    writeNinjaFile(*index);
    //    return std::system(
    //        ("ninja -f " +
    //         (std::filesystem::path{cachePath} / "build.ninja").string())
    //            .c_str());
}
