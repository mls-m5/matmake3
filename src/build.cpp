#include "build.h"
#include "buildcontext.h"
#include "commandstream.h"
#include <iostream>
#include <unordered_set>

namespace {

void buildObj(BuildContext &context, File &file) {
    //    std::cerr << "fake compiling... " << file.fullPath() << std::endl;

    Command{} << context.compiler << " " << file.src->fullPath() << " -c "
              << file.fullPath();
}

void buildExe(BuildContext &context, File &file) {
    //    std::cerr << "fake linking... " << file.fullPath() << std::endl;

    Command{} << context.compiler << file.dependencies << " -o "
              << file.fullPath();
}

void buildPcm(BuildContext &context, File &file) {
    //    std::cerr << "fake pcm build... " << file.fullPath() << std::endl;

    Command{} << context.compiler << " " << file.src->fullPath()
              << " --precompile -o " << file.fullPath();
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
