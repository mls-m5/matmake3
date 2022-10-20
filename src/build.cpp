#include "build.h"
#include "buildcontext.h"
#include "commandstream.h"
#include <iostream>
#include <unordered_set>

namespace {

std::string pcmDepString(File &file) {
    auto depss = std::ostringstream{};

    for (auto dep : file.dependencies) {
        if (dep->fullPath().extension() == ".pcm") {
            depss << " -fmodule-file=" << dep->fullPath();
        }
    }

    return depss.str();
}

void buildObj(BuildContext &context, File &file) {
    (Command{} << context.compiler << " " << file.src->fullPath()
               << pcmDepString(file) << " -c -o " << file.fullPath())
        .run();
}

void buildExe(BuildContext &context, File &file) {
    (Command{} << context.compiler << file.dependencies << " -o "
               << file.fullPath())
        .run();
}

void buildPcm(BuildContext &context, File &file) {
    (Command{} << context.compiler << " " << file.src->fullPath()
               << pcmDepString(file) << " --precompile -o " << file.fullPath())
        .run();
}

BuildContext buildContext() {
    return {{
        {".o", buildObj},
        {"exe", buildExe},
        {".pcm", buildPcm},
    }};
}

} // namespace

void build(Target &target) {
    auto out = target.output();

    auto context = buildContext();
    createBuildPaths(target.index());
    context.build(*out);
}

void createBuildPaths(const Index &index) {
    // unordered map does not seem to support filesystem paths
    auto dirs = std::unordered_set<std::string>{};

    for (auto &file : index.files) {
        if (!file->isSource()) {
            dirs.insert(file->fullPath().parent_path().string());
        }
    }

    for (auto &dir : dirs) {
        //        std::cout << "should create dir " << dir << std::endl;
        std::filesystem::create_directories(dir);
    }
}
