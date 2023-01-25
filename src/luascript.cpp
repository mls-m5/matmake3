#include "luascript.h"
#include "sol/sol.hpp"
#include <filesystem>
#include <iostream>

namespace {

void addObject(Target &target, std::filesystem::path src) {
    auto objSrc = std::filesystem::path{src.string() + ".o"};
    auto obj = target.requestObject(objSrc, src);
    target.addObject(obj);
}

void src(Target &target, const Index &index, sol::object src) {
    if (src.is<std::string>()) {
        auto path = src.as<std::string>();
        if (path.find("*") != std::string::npos) {
            for (auto file : index.findGlob(path)) {
                addObject(target, file->path);
            }
        }
    }
    else if (src.is<sol::table>()) {
        sol::table table = src.as<sol::table>();
        for (auto path : table) {
            // Recurse through list
            ::src(target, index, path.second);
        }
    }
    else {
        throw sol::error{"src is not string or array"};
    }
}

} // namespace

bool hasBuildScript(const BuildPaths &paths) {
    return std::filesystem::exists(paths.buildScript);
}

void runBuildScript(const BuildPaths &paths,
                    const Index &index,
                    Target &target) {
    auto lua = sol::state{};
    lua.set_function("executable", [&target, &index](sol::table value) {
        src(target, index, value.get<sol::object>("src"));
    });

    auto script = lua.load_file(paths.buildScript);
    auto res = script();
    if (!res.valid()) {
        std::cerr << "error when executing build script\n";
        std::terminate();
    }

    target.output()->buildType = "exe";
}
