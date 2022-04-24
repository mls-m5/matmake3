#pragma once

#include "filter.h"
#include <filesystem>

struct File {
    enum Type {
        Unknown,
        Source,
    };

    std::filesystem::path path;
    Type type = Unknown;

    File(std::filesystem::path p)
        : path{std::move(p)} {
        if (isSourceFile(path)) {
            type = Source;
        }
    }

    bool isSource() const {
        return type == Source;
    }
};
