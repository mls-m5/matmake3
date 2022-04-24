#include "filter.h"

bool isHeaderFile(const std::filesystem::__cxx11::path &path) {
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

bool isSourceFile(const std::filesystem::__cxx11::path &path) {
    auto ext = path.extension();
    static constexpr auto arr = std::array{
        ".cpp",
    };
    for (auto a : arr) {
        if (a == ext) {
            return true;
        }
    }

    return false;
}

bool shouldInclude(const std::filesystem::__cxx11::path &path) {
    return isSourceFile(path) || isHeaderFile(path);
}
