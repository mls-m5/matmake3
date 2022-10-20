#pragma once

#include "commandstream.h"
#include "file.h"
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
    BuildContext(BuildFMap map)
        : map{std::move(map)} {}

    BuildFMap map;

    std::string compiler = "clang++-16";
    std::string flags = "-std=c++20";
    std::string linkFlags = "";

    std::string common() {
        return compiler + " " + flags;
    }

    void build(File &file) {
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

        std::cout << "build " << file.fullPath().filename().string()
                  << std::endl;

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
    void run(const File &file, Args... args) {
        auto ss = std::ostringstream{};
        ((ss << args << " "), ...);
        _commandList.commands.push_back({ss.str(), file});
    }

    const CommandList &commandList() {
        return _commandList;
    }

private:
    void buildDeps(File &file) {
        for (auto dep : file.dependencies) {
            build(*dep);
        }
    }

    CommandList _commandList;
};
