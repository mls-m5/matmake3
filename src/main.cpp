#include "build.h"
#include "createrecursive.h"
#include "index.h"
#include "log.h"
#include "settings.h"
#include <iostream>

int main(int argc, char *argv[]) {
    const auto settings = Settings{argc, argv};

    mlog.enableDebugOutput(settings.isVerbose);

    std::ios::sync_with_stdio(false);

    auto index = std::make_unique<Index>();
    auto target = createRecursive(*index, settings.paths);

    if (settings.command == Settings::Build) {
        try {
            build(*target, settings);
            std::cout << "done...\n";
        }
        catch (std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }
    else if (settings.command) {
        clean(*target, settings);
    }
}
