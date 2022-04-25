#include "LuaCpp.hpp"
#include "index.h"
#include "ninja.h"
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);

    auto lua = LuaCpp::LuaContext{};

    // The simples way is to use CompileStringAndRun method
    try {
        lua.CompileStringAndRun(
            "print('The fastest way to start using lua in a project')");
    }
    catch (std::runtime_error &e) {
        std::cout << e.what() << '\n';
    }

    auto index = Index{};

    writeNinjaFile(index);

    auto cachePath = "build/.mm3/default";

    return std::system(
        ("ninja -f " +
         (std::filesystem::path{cachePath} / "build.ninja").string())
            .c_str());
}
