#pragma once

#include "filter.h"
#include "nlohmann/json.hpp"
#include "uniqueid.h"
#include <filesystem>
#include <vector>

struct File {
    enum Type {
        Unknown,
        Source,
        Intermediate,
        Output,
    };

    ~File() = default;
    File(std::filesystem::path p, Type type)
        : path{std::move(p)}
        , type{type} {
        //        if (isSourceFile(path)) {
        //            type = Source;
        //        }
    }

    File(const File &) = delete;
    File(File &&) = delete;
    File operator=(const File &) = delete;
    File operator=(File &&) = delete;

    bool isSource() const {
        return type == Source;
    }

    // Where to put file if it were in the same path
    std::filesystem::path sameDir(std::filesystem::path path) {
        return this->path.parent_path() / path.filename();
    }

    friend void to_json(nlohmann::json &j, const File &file);

    std::filesystem::path path;
    Type type = Unknown;
    std::vector<File *> dependencies;
    Id id = uniqueId();
};

NLOHMANN_JSON_SERIALIZE_ENUM(File::Type,
                             {
                                 {File::Unknown, "unknown"},
                                 {File::Source, "source"},
                                 {File::Intermediate, "intermediate"},
                                 {File::Output, "output"},
                             })

inline void to_json(nlohmann::json &j, const File *f) {
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
        {"id", file.id},
        {"deps", file.dependencies},
        {"type", file.type},
    };
}

inline void to_json(nlohmann::json &j, const std::unique_ptr<File> &f) {
    to_json(j, *f);
}
