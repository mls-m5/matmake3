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

void include(Target &target, sol::object path) {
    if (!path) {
        return;
    }
    if (path.is<std::string>()) {
        target.addInclude(path.as<std::string>());
    }
    else if (path.is<sol::table>()) {
        sol::table table = path.as<sol::table>();
        for (auto path : table) {
            ::include(target, path.second);
        }
    }
    else {
        throw sol::error{"include is not string or array"};
    }
}

void name(Target &target, sol::object name) {
    if (!name) {
        return;
    }

    target.name(name.as<std::string>());
}

void flags(Target &target, sol::object f) {
    if (!f) {
        return;
    }
    if (f.is<std::string>()) {
        auto flags = f.as<std::string>();
        target.addFlags(flags);
    }
    else if (f.is<sol::table>()) {
        for (auto flag : f.as<sol::table>()) {
            flags(target, flag.second);
        }
    }
}

void link(Target &target, sol::object l) {
    if (!l) {
        return;
    }
    if (l.is<std::string>()) {
        target.addLink(l.as<std::string>());
    }
    else if (l.is<sol::table>()) {
        for (auto flag : l.as<sol::table>()) {
            link(target, flag.second);
        }
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
        include(target, value.get<sol::object>("include"));
        flags(target, value.get<sol::object>("flags"));
        name(target, value.get<sol::object>("name"));
        link(target, value.get<sol::object>("link"));
    });

    auto script = lua.load_file(paths.buildScript);
    auto res = script();
    if (!res.valid()) {
        std::cerr << "error when executing build script\n";
        std::terminate();
    }

    target.output()->buildType = "exe";
}
