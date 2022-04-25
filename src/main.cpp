//#include "LuaCpp.hpp"
#include "index.h"

extern "C" {
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"
}
#include <filesystem>
#include <iostream>
//#include <lauxlib.h>
//#include <lua.h>
//#include <lualib.h>
#include "ninja.h"

auto standardCode =
    R"_(

--executables = {
--    main = {
--        src = std.glob("src/**.cpp")
--    }
--}

executables = glob("src/*.cpp");
test = glob();
)_";

auto fileIndex = std::shared_ptr<Index>{};

extern "C" {
int glob(lua_State *L) {
    //    auto str = luaL_checkstring(L, 1);
    if (lua_isstring(L, 1)) {
        std::cout << "has arguments " << lua_tostring(L, 1) << "\n";
    }
    else {
        std::cout << "does not have arguments\n";
    }

    lua_newtable(L);

    int index = 1;

    for (auto &file : fileIndex->files) {
        if (!file.isSource()) {
            continue;
        }
        lua_pushinteger(L, index);
        lua_pushstring(L, file.path.string().c_str());
        lua_settable(L, -3);

        ++index;
    }

    return 1;
}
}

std::vector<std::filesystem::path> getLuaTablePaths(lua_State *L) {
    auto paths = std::vector<std::filesystem::path>{};

    for (int i = 0;; ++i) {
        lua_pushnumber(L, i + 1);
        lua_gettable(L, -2);

        if (lua_isnil(L, -1)) {
            break;
        }

        paths.push_back(lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    return paths;
}

void runLua() {
    auto L = luaL_newstate();

    luaL_openlibs(L);

    lua_pushcfunction(L, glob);
    lua_setglobal(L, "glob");

    luaL_dostring(L, standardCode);

    lua_getglobal(L, "test");

    auto paths = getLuaTablePaths(L);

    lua_pop(L, 1);

    for (auto &path : paths) {
        std::cout << path << "\n";
    }
}

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);

    fileIndex = std::make_shared<Index>();

    runLua();

    writeNinjaFile(*fileIndex);

    auto cachePath = "build/.mm3/default";

    return std::system(
        ("ninja -f " +
         (std::filesystem::path{cachePath} / "build.ninja").string())
            .c_str());
}
