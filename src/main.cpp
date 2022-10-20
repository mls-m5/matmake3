#include "build.h"
#include "createrecursive.h"
#include "index.h"
// #include "ninja.h"
#include "settings.h"
#include <iostream>

int main(int argc, char *argv[]) {
    const auto settings = Settings{argc, argv};
    std::ios::sync_with_stdio(false);

    auto index = std::make_unique<Index>();
    auto target = createRecursive(*index);

    try {
        build(*target, settings);
    }
    catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    //    writeNinjaFile(*index);
    //    return std::system(
    //        ("ninja -f " +
    //         (std::filesystem::path{cachePath} / "build.ninja").string())
    //            .c_str());
}
