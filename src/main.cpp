#include <filesystem>
#include <index.h>
#include <iostream>

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);

    auto index = Index{};

    std::cout << std::filesystem::current_path() << std::endl;

    for (auto &file : index.files) {
        std::cout << file.path << "\n";
    }

    return 0;
}
