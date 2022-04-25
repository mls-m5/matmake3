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

// executables = {
//     main = {
//         src = std.glob("src/**.cpp")
//     }
// }

// print(executables['main']['src'][0])
auto standardCode =
    R"_(
--test = {}
--test[0] = "hello"
--test[1] = "there"
--test["x"] = "there"

test = {"hello", "there", "you"}
)_";

auto fileIndex = std::shared_ptr<Index>{};

extern "C" {
int glob(lua_State *L) {
    lua_newtable(L);
    for (auto &file : fileIndex->files) {
        if (!file.isSource()) {
            continue;
        }
        lua_pushinteger(L, 1);
        lua_pushstring(L, file.path.string().c_str());
        lua_settable(L, -3);
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

        //        std::cout << lua_tostring(L, -1) << std::endl;
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

    for (auto &path : paths) {
        std::cout << path << "\n";
    }

    lua_pop(L, 1);
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
