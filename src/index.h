#pragma once

#include "filter.h"
#include <file.h>
#include <vector>

struct Index {
    std::vector<File> files;

    Index() {
        for (auto &it : std::filesystem::recursive_directory_iterator{"."}) {
            if (it.is_regular_file() && shouldInclude(it.path())) {
                files.push_back({std::filesystem::relative(it.path(), ".")});
            }
        }
    }
};
