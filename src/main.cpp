#include "LuaCpp.hpp"
#include "index.h"
#include "ninja.h"
#include <filesystem>
#include <iostream>

using namespace std::literals;

auto standardCode =
    R"_(

executables = {
    main = {
        src = std.glob("src/**.cpp")
    }
}

print(executables['main']['src'][0])

)_"s;

auto index = std::shared_ptr<Index>{};

extern "C" {
int _glob(lua_State *L) {
    lua_newtable(L);
    for (auto &file : index->files) {
        if (!file.isSource()) {
            continue;
        }
        lua_pushinteger(L, 0);
        lua_pushstring(L, file.path.string().c_str());
        lua_settable(L, -3);
    }
    return 1;
}
}

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);

    index = std::make_shared<Index>();

    auto lua = LuaCpp::LuaContext{};

    auto lib = std::make_shared<LuaCpp::Registry::LuaLibrary>("std");
    lib->AddCFunction("glob", _glob);

    lua.AddLibrary(lib);

    // The simples way is to use CompileStringAndRun method
    try {
        lua.CompileStringAndRun(
            "print('The fastest way to start using lua in a project')");
        lua.CompileStringAndRun(standardCode);
    }
    catch (std::runtime_error &e) {
        std::cout << e.what() << '\n';
    }

    writeNinjaFile(*index);

    auto cachePath = "build/.mm3/default";

    return std::system(
        ("ninja -f " +
         (std::filesystem::path{cachePath} / "build.ninja").string())
            .c_str());
}
