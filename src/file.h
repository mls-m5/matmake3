#pragma once

#include "buildpaths.h"
#include "nlohmann/json.hpp"
#include "uniqueid.h"
#include <filesystem>
#include <ostream>
#include <vector>

struct File {
    enum Type {
        Unknown,
        Source,
        Header,
        Intermediate,
        Output,
    };

    ~File() = default;
    File(std::filesystem::path p, std::filesystem::path fullPath, Type type)
        : path{std::move(p)}
        , fullPath{std::move(fullPath)}
        , type{type} {}

    File(const File &) = delete;
    File(File &&) = delete;
    File operator=(const File &) = delete;
    File operator=(File &&) = delete;

    bool isSource() const {
        return type == Source;
    }

    // Where to put file if it were in the same path
    std::filesystem::path sameDir(std::filesystem::path path) const {
        return this->path.parent_path() / path.filename();
    }

    friend void to_json(nlohmann::json &j, const File &file);

    std::filesystem::path path;
    std::filesystem::path fullPath;
    Type type = Unknown;
    std::vector<File *> dependencies;
    File *src = nullptr;
    Id id = uniqueId();
    bool isBuilt = false;       // Only used for when building in application
    std::string buildType = ""; // Override file extension
};

NLOHMANN_JSON_SERIALIZE_ENUM(File::Type,
                             {
                                 {File::Unknown, "unknown"},
                                 {File::Source, "source"},
                                 {File::Header, "source"},
                                 {File::Intermediate, "intermediate"},
                                 {File::Output, "output"},
                             })

inline void to_json(nlohmann::json &j, const File *f) {
    if (!f) {
        j = nlohmann::json{};
        return;
    }

    if (false) {
        j = f->id;
    }
    else {
        j = f->path;
    }
}

inline void to_json(nlohmann::json &j, const File &file) {
    j = {
        {"path", file.path},
        {"fullPath", file.fullPath},
        {"id", file.id},
        {"deps", file.dependencies},
        {"src", file.src},
        {"type", file.type},
        {"buildType", file.buildType},
    };
}

inline void to_json(nlohmann::json &j, const std::unique_ptr<File> &f) {
    to_json(j, *f);
}

inline std::filesystem::path fullPath(const BuildPaths &paths,
                                      const std::filesystem::path &path,
                                      File::Type type) {
    //    if (!file.alias.empty()) {
    //        return file.alias;
    //    }

    switch (type) {
    case File::Source:
    case File::Header:
        return path;
    case File::Intermediate:
        return paths.cache / path;
    case File::Output:
        return paths.out / path;
    default:
        throw std::runtime_error{"invalid file " + path.string()};
    }
}

inline std::ostream &operator<<(std::ostream &stream,
                                const std::vector<File *> files) {
    for (auto &file : files) {
        stream << " " << file->fullPath;
    }
    return stream;
}
