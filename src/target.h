#pragma once
#include "file.h"
#include "index.h"
#include <functional>
#include <unordered_map>

// Target is a single file context, like a executable or library
class Target {
public:
    Target(const Target &) = delete;
    Target(Target &&) = delete;
    Target &operator=(const Target &) = delete;
    Target &operator=(Target &&) = delete;

    Target(Index &index)
        : _index{&index} {}

    auto &index() {
        return *_index;
    }

    // Try to find an object, and create with the right function if it does not
    // exist
    File *requestObject(std::filesystem::path path) {
        if (auto f = _index->find(path)) {
            return f;
        }
        else {
            if (auto func = _createFunctions.find(path.extension());
                func != _createFunctions.end()) {
                auto file = func->second(*this, path);

                if (!file) {
                    throw std::runtime_error{"could not create file " +
                                             path.string()};
                }

                _files.push_back(file);
                return file;
            }
        }

        return nullptr;
    }

    using CreateT =
        std::function<File *(Target &target, std::filesystem::path path)>;

    void registerFunction(std::filesystem::path extension, CreateT f) {
        _createFunctions[extension] = f;
    }

    File *find(std::filesystem::path path) {
        return _index->find(path);
    }

private:
    Index *_index;

    std::vector<File *> _files;
    std::unordered_map<std::string, CreateT> _createFunctions;
};
