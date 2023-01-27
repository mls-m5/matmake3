#pragma once

#include "nlohmann/json.hpp"
#include <filesystem>
#include <ostream>
#include <vector>

// representation of a file that could be included in the project
struct File {
    ~File() = default;
    File(std::filesystem::path p, std::filesystem::path fullPath)
        : path{std::move(p)}
        , fullPath{std::move(fullPath)} {}

    File(const File &) = delete;
    File(File &&) = delete;
    File operator=(const File &) = delete;
    File operator=(File &&) = delete;

    bool isSource() const {
        return !src && dependencies.empty();
    }

    // Where to put file if it were in the same path
    std::filesystem::path sameDir(std::filesystem::path path) const {
        return this->path.parent_path() / path.filename();
    }

    friend void to_json(nlohmann::json &j, const File &file);

    std::filesystem::path depFile() const {
        return fullPath.string() + ".d";
    }

    void name(std::string n) {
        path.replace_filename(n);
        fullPath.replace_filename(n);
    }

    std::filesystem::path path;
    std::filesystem::path fullPath;
    std::vector<File *> dependencies;
    File *src = nullptr;
    bool isBuilt = false;       // Only used for when building in application
    std::string buildType = ""; // Override file extension
    bool shouldUseDepFile = false;
};

inline void to_json(nlohmann::json &j, const File *f) {
    if (!f) {
        j = nlohmann::json{};
        return;
    }

    j = f->path;
}

inline void to_json(nlohmann::json &j, const File &file) {
    j = {
        {"path", file.path},
        {"fullPath", file.fullPath},
        {"deps", file.dependencies},
        {"src", file.src},
        {"buildType", file.buildType},
        {"shouldUseDepFile", file.shouldUseDepFile},
    };
}

inline void to_json(nlohmann::json &j, const std::unique_ptr<File> &f) {
    to_json(j, *f);
}

inline std::ostream &operator<<(std::ostream &stream,
                                const std::vector<File *> files) {
    for (auto &file : files) {
        stream << " " << file->fullPath;
    }
    return stream;
}
