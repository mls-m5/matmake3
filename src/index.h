#pragma once

#include "file.h"
#include "filter.h"
#include <memory>
#include <vector>

struct Index {
    std::vector<std::unique_ptr<File>> files;

    Index() {
        for (auto &it : std::filesystem::recursive_directory_iterator{"."}) {
            if (it.is_regular_file() && shouldInclude(it.path())) {
                files.push_back(std::make_unique<File>(
                    std::filesystem::relative(it.path(), ".")));
            }
        }
    }

    File *getMainSrc() {
        for (auto &f : files) {
            if (f->path.stem() == "main") {
                return f.get();
            }
        }
        return nullptr;
    }
};
