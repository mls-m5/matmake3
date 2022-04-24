#include "index.h"
#include "ninja.h"
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);

    auto index = Index{};

    writeNinjaFile(index);

    std::system("ninja");

    return 0;
}
