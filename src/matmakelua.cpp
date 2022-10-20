

extern "C" {
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"
}

#include "glob.h"
#include "index.h"
#include "matmakelua.h"
#include "ninja.h"
#include <filesystem>
#include <iostream>

auto standardCode =
    R"_(

--exe = {
--    main = {
--        src = std.glob("src/**.cpp")
--    }
--}

src = glob("src/*.cpp");

test = glob();

project({
    exe = {
        main =
    }
})

)_";

namespace {

auto fileIndex = (Index *){};

}

extern "C" {
int glob(lua_State *L) {
    auto res = std::vector<std::regex>{};
    if (lua_isstring(L, 1)) {
        std::cout << "has arguments " << lua_tostring(L, 1) << "\n";
        res.push_back(glob2Regex(lua_tostring(L, 1)));
    }
    else {
        std::cout << "does not have arguments\n";
    }

    lua_newtable(L);

    int index = 1;

    auto pushToTable = [L, &index](const std::filesystem::path &path) {
        lua_pushinteger(L, index);
        lua_pushstring(L, path.string().c_str());
        lua_settable(L, -3);
    };

    if (res.empty()) {
        for (auto &file : fileIndex->files) {
            if (!file->isSource()) {
                continue;
            }

            pushToTable(file->path);

            ++index;
        }
    }
    else {
        for (auto &file : fileIndex->files) {
            if (!file->isSource()) {
                continue;
            }

            bool isMatch = false;

            for (auto &re : res) {
                if (std::regex_match(file->path.string(), re)) {
                    isMatch = true;
                    break;
                }
            }

            if (!isMatch) {
                continue;
            }

            std::cout << "matching: " << file->path << std::endl;

            pushToTable(file->path);

            ++index;
        }
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

void runLua(Index &index) {
    ::fileIndex = &index;
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
