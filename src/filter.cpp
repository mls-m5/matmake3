#include "filter.h"

bool isHeaderFile(const std::filesystem::path &path) {
    auto ext = path.extension();
    static constexpr auto arr = std::array{
        ".h",
    };
    for (auto a : arr) {
        if (a == ext) {
            return true;
        }
    }

    return false;
}

bool isSourceFile(const std::filesystem::path &path) {
    auto ext = path.extension();
    static constexpr auto arr = std::array{
        ".cpp",
        ".cppm",
    };
    for (auto a : arr) {
        if (a == ext) {
            return true;
        }
    }

    return false;
}

bool shouldInclude(const std::filesystem::path &path) {
    return isSourceFile(path) || isHeaderFile(path);
}
