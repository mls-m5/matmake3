#pragma once

#include "file.h"
#include "filter.h"
#include "nlohmann/json.hpp"
#include <memory>
#include <unordered_set>
#include <vector>

struct Index {
    std::vector<std::unique_ptr<File>> files;

    Index() {
        for (auto &it : std::filesystem::recursive_directory_iterator{"."}) {
            if (it.is_regular_file() && shouldInclude(it.path())) {
                files.push_back(std::make_unique<File>(
                    std::filesystem::relative(it.path(), "."),
                    isSourceFile(it.path()) ? File::Source : File::Unknown));
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

    File *find(std::filesystem::path path) {
        for (auto &f : files) {
            if (f->path == path) {
                return f.get();
            }
        }
        for (auto &f : files) {
            if (f->path.filename() == path) {
                return f.get();
            }
        }
        return nullptr;
    }

    std::vector<File *> findAll(std::string extension) {
        auto files = std::vector<File *>{};
        for (auto &f : this->files) {
            if (f->path.extension() == extension) {
                files.push_back(f.get());
            }
        }

        return files;
    }

    friend void to_json(nlohmann::json &j, const Index &i) {
        j = nlohmann::json{{"files", i.files}};
    }
};
