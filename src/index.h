#pragma once

#include "file.h"
#include "filter.h"
#include "glob.h"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <memory>
#include <unordered_set>
#include <vector>

//! Summary of all files that could be related to the project in the current
//! folder
struct Index {
    std::vector<std::unique_ptr<File>> files;

    Index();

    File *find(std::filesystem::path path) const;

    std::vector<File *> findAll(std::string extension) const;

    std::vector<File *> findGlob(std::string glob) const;

    File *findSystemFile(std::filesystem::path path);

    friend void to_json(nlohmann::json &j, const Index &i) {
        j = nlohmann::json{{"files", i.files}};
    }
};
