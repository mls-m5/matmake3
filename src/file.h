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
        Header,
        Intermediate,
        Output,
    };

    ~File() = default;
    File(std::filesystem::path p, Type type)
        : path{std::move(p)}
        , type{type} {}

    File(const File &) = delete;
    File(File &&) = delete;
    File operator=(const File &) = delete;
    File operator=(File &&) = delete;

    bool isSource() const {
        return type == Source;
    }

    std::filesystem::path fullPath() {
        if (!alias.empty()) {
            return alias;
        }

        auto cachePath = "build/.mm3/default";
        auto outPath = "build/default";

        switch (type) {
        case Source:
        case Header:
            return path;
        case Intermediate:
            return cachePath / path;
        case Output:
            return outPath / path;
        default:
            throw std::runtime_error{"invalid file " + path.string()};
        }
    }

    // Where to put file if it were in the same path
    std::filesystem::path sameDir(std::filesystem::path path) {
        return this->path.parent_path() / path.filename();
    }

    friend void to_json(nlohmann::json &j, const File &file);

    std::filesystem::path path;
    std::filesystem::path alias;
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
        {"id", file.id},
        {"deps", file.dependencies},
        {"src", file.src},
        {"type", file.type},
        {"buildType", file.buildType},
        {"alias", file.alias},
    };
}

inline void to_json(nlohmann::json &j, const std::unique_ptr<File> &f) {
    to_json(j, *f);
}

inline std::ostream &operator<<(std::ostream &stream,
                                const std::vector<File *> files) {
    for (auto &file : files) {
        stream << " " << file->fullPath();
    }
    return stream;
}
