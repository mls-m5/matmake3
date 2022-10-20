#pragma once
#include "file.h"
#include "index.h"

// Target is a single file context, like a executable or library
class Target {
public:
    Target(Index &index)
        : _index{&index} {}

    auto &index() {
        return *_index;
    }

private:
    Index *_index;

    std::vector<File *> _files;
};
