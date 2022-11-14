#include "build.h"
#include "buildcontext.h"
#include "buildpaths.h"
#include "commandlisit.h"
#include "compilationdatabase.h"
#include "ninja.h"
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>

namespace {

std::string pcmDepString(const BuildContext &context, const File &file) {
    auto depss = std::ostringstream{};

    for (auto dep : file.dependencies) {
        const auto fullPath = dep->fullPath;
        if (fullPath.extension() == ".pcm") {
            if (false) { // Should be used for headers (maybe everything when it
                         // works)
                depss << " -fmodule-file=" << fullPath.stem().string() << "="
                      << fullPath;
            }
            else {
                depss << " -fmodule-file=" << fullPath;
            }
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
    context.run(file,
                context.common(),
                file.src->fullPath,
                pcmDepString(context, file),
                "-c -o",
                file.fullPath);
}

void buildExe(BuildContext &context, File &file) {
    context.run(file,
                context.common(),
                context.linkFlags,
                file.dependencies,
                "-o",
                file.fullPath);
}

void buildPcm(BuildContext &context, File &file) {
    context.run(file,
                context.common(),
                file.src->fullPath,
                pcmDepString(context, file),
                "-MD -MF",
                file.depFile(),
                "--precompile -o",
                file.fullPath);
}

void buildHeaderPcm(BuildContext &context, File &file) {
    context.run(file,
                context.common(),
                file.src->fullPath,
                "-MD -MF",
                file.depFile(),
                "-fmodule-header -o",
                file.fullPath);
}

void buildSysHeaderPcm(BuildContext &context, File &file) {
    if (true) {
        context.run(file,
                    context.common(),
                    "-xc++-system-header --precompile",
                    //                    file.src->fullPath,
                    //                    file.src->fullPath.stem().string(),
                    file.src->path.string(),
                    "-o",
                    file.fullPath,
                    "-Wno-pragma-system-header-outside-header",
                    "-Wno-user-defined-literals");
    }
    else {

        context.run(file,
                    context.common(),
                    "-fmodule-header=system -xc++-header",
                    file.src->fullPath.stem(),
                    "-o",
                    file.fullPath,
                    "-Wno-pragma-system-header-outside-header",
                    "-Wno-user-defined-literals");
    }
}

BuildContext buildContext(const BuildPaths &paths) {
    auto context = BuildContext{
        {
            {".o", buildObj},
            {"exe", buildExe},
            {".pcm", buildPcm},
            {".h.pcm", buildHeaderPcm},
            {"sys.h.pcm", buildSysHeaderPcm},
        },
        paths,
    };

    return context;
}

} // namespace

void build(Target &target, const Settings &settings) {
    auto out = target.output();

    auto context = buildContext(settings.paths);
    context.flags += flags(target);
    context.linkFlags += settings.linkFlags;
    context.flags += " " + settings.cxxflags;

    createBuildPaths(target, context);
    context.build(*out);

    writeCompilationDatabase(context.paths(), context.commandList());

    if (settings.shouldBuildNative) {
        for (auto &command : context.commandList().commands()) {
            std::cout << "build " << command.file.fullPath.filename()
                      << std::endl;
            std::cout << command.command << std::endl;
            if (std::system(command.command.c_str())) {
                throw std::runtime_error{"failed with command: " +
                                         command.command};
            }
        }
        return;
    }

    writeNinjaFile(context.paths(), context.commandList());
}

void createBuildPaths(const Target &target, BuildContext &context) {
    // unordered map does not seem to support filesystem paths
    auto dirs = std::unordered_set<std::string>{};

    for (auto &file : target.files()) {
        if (!file->isSource()) {
            dirs.insert(file->fullPath.parent_path().string());
        }
    }

    for (auto &dir : dirs) {
        if (dir.empty()) {
            continue;
        }
        std::cout << "creating directory " << dir << std::endl;
        std::filesystem::create_directories(dir);
    }
}
