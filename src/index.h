#pragma once

#include "file.h"
#include "filter.h"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <memory>
#include <unordered_set>
#include <vector>

struct Index {
    std::vector<std::unique_ptr<File>> files;

    static auto type(const std::filesystem::path &path) {
        if (isHeaderFile(path)) {
            return File::Header;
        }
        return isSourceFile(path) ? File::Source : File::Unknown;
    };

    Index() {

        for (auto it = std::filesystem::recursive_directory_iterator{"."};
             it != decltype(it){};
             ++it) {
            if (it->path().filename() == "build") {
                it.disable_recursion_pending();
            }
            if (it->is_regular_file() && shouldInclude(it->path())) {
                auto rpath = std::filesystem::relative(it->path(), ".");
                files.push_back(
                    std::make_unique<File>(rpath, rpath, type(it->path())));
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

    File *findSystemFile(std::filesystem::path path) {
        // TODO: Do something more generic
        auto libcHeaders = "/usr/lib/llvm-16/include/c++/v1/";

        if (auto file = find(path)) {
            return file;
        }

        auto fullPath = libcHeaders / path;
        if (std::filesystem::exists(fullPath)) {
            files.push_back(std::make_unique<File>(path, fullPath, type(path)));
            files.back()->alias = fullPath;
            return files.back().get();
        }

        fullPath = "/usr/include" / path;
        if (std::filesystem::exists(fullPath)) {
            files.push_back(std::make_unique<File>(path, fullPath, type(path)));
            files.back()->alias = fullPath;
            return files.back().get();
        }
        return nullptr;
    }

    friend void to_json(nlohmann::json &j, const Index &i) {
        j = nlohmann::json{{"files", i.files}};
    }
};
