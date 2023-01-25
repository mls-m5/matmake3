#pragma once

#include "buildpaths.h"
#include "commandlisit.h"
#include "file.h"
#include "log.h"
#include <functional>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>

struct BuildContext;

using BuildFMap =
    std::unordered_map<std::string,
                       std::function<void(BuildContext &, File &)>>;

// Rules how to build each file
// Might be replaced by some json template magick later
struct BuildContext {
    BuildContext(BuildFMap map, const BuildPaths &paths)
        : map{std::move(map)}
        , _paths{paths} {}

    BuildFMap map;

    std::string compiler = "clang++-16";
    std::string flags = "-std=c++20 -stdlib=libc++";
    std::string linkFlags = "";

    std::string common() {
        return compiler + " " + flags;
    }

    void build(File &file);

    template <typename... Args>
    void run(const File &file, Args... args);

    const CommandList &commandList() const {
        return _commandList;
    }

    const auto &paths() const {
        return _paths;
    }

private:
    void buildDeps(File &file) {
        for (auto dep : file.dependencies) {
            if (dep == &file) {
                continue;
            }
            build(*dep);
        }
    }

    CommandList _commandList;
    const BuildPaths &_paths;
};

inline void BuildContext::build(File &file) {
    if (file.isSource() || (!file.src && file.dependencies.empty())) {
        return;
    }
    if (file.isBuilt) {
        return;
    }

    buildDeps(file);
    if (file.src) {
        build(*file.src);
    }

    mlog.debug("prepare ", file.path.filename());

    auto buildType = file.buildType;

    auto ext = file.path.extension();

    if (buildType.empty()) {
        buildType = ext;
    }

    try {
        auto &f = map.at(buildType);
        f(*this, file);
    }
    catch (std::out_of_range &e) {
        throw std::runtime_error{"no rule for type " + buildType};
    }

    file.isBuilt = true;
}

template <typename... Args>
void BuildContext::run(const File &file, Args... args) {
    auto ss = std::ostringstream{};
    ((ss << args << " "), ...);
    _commandList.add(ss.str(), file, file.fullPath);
}
