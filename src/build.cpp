#include "build.h"
#include "buildcontext.h"
#include "commandstream.h"
#include <iostream>
#include <sstream>
#include <string>
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

std::string flags(Target &target) {
    auto ss = std::ostringstream{};
    for (auto i : target.includes()) {
        ss << " -I" << i;
    }
    return ss.str();
}

void buildObj(BuildContext &context, File &file) {
    (Command{} << context.common() << " " << file.src->fullPath()
               << pcmDepString(file) << " -c -o " << file.fullPath())
        .run();
}

void buildExe(BuildContext &context, File &file) {
    (Command{} << context.common() << " " << context.linkFlags << " "
               << file.dependencies << " -o " << file.fullPath())
        .run();
}

void buildPcm(BuildContext &context, File &file) {
    (Command{} << context.common() << " " << file.src->fullPath()
               << pcmDepString(file) << " --precompile -o " << file.fullPath())
        .run();
}

void buildHeaderPcm(BuildContext &context, File &file) {
    (Command{} << context.common() << " " << file.src->fullPath()
               << " -fmodule-header -o " << file.fullPath())
        .run();
}

BuildContext buildContext() {
    auto context = BuildContext{{
        {".o", buildObj},
        {"exe", buildExe},
        {".pcm", buildPcm},
        {".h.pcm", buildHeaderPcm},
    }};

    return context;
}

} // namespace

void build(Target &target, const Settings &settings) {
    auto out = target.output();

    auto context = buildContext();
    context.flags += flags(target);
    context.linkFlags += settings.linkFlags;

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
        if (dir.empty()) {
            continue;
        }
        std::filesystem::create_directories(dir);
    }
}
