#pragma once

#include "filter.h"
#include <filesystem>
#include <vector>

struct File {
    ~File() = default;
    File(std::filesystem::path p)
        : path{std::move(p)} {
        if (isSourceFile(path)) {
            type = Source;
        }
    }

    File(const File &) = delete;
    File(File &&) = delete;
    File operator=(const File &) = delete;
    File operator=(File &&) = delete;

    enum Type {
        Unknown,
        Source,
        Intermediate,
        Output,
    };

    std::filesystem::path path;
    Type type = Unknown;

    bool isSource() const {
        return type == Source;
    }

    std::vector<File *> dependencies;
};
