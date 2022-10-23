#pragma once
#include "buildpaths.h"
#include "file.h"
#include "index.h"
#include "nlohmann/json.hpp"
#include <algorithm>
#include <filesystem>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// Target is a single file context, like a executable or library
class Target {
public:
    Target(const Target &) = delete;
    Target(Target &&) = delete;
    Target &operator=(const Target &) = delete;
    Target &operator=(Target &&) = delete;

    Target(std::string name, Index &index, const BuildPaths &paths)
        : _index{&index}
        , _paths{paths} {
        _output = createOutputFile(name);
    }

    // Try to find an object, and create with the right function if it does not
    // exist
    // Request name is the name from where the path is generated
    File *requestObject(std::filesystem::path path,
                        std::filesystem::path requestName,
                        std::string type = "") {
        if (auto f = _index->find(path)) {
            return f;
        }
        else {
            type = type.empty() ? path.extension().string() : type;
            if (auto func = _createFunctions.find(type);
                func != _createFunctions.end()) {
                auto file = [&]() {
                    try {
                        return func->second(*this, path, requestName);
                    }
                    catch (std::runtime_error &e) {
                        throw std::runtime_error{"in " + path.string() + " - " +
                                                 requestName.string() + ":\n" +
                                                 e.what()};
                    }
                }();

                if (!file) {
                    throw std::runtime_error{"could not create file " +
                                             path.string()};
                }
                return file;
            }
        }

        throw std::runtime_error{"no known way to create file for " +
                                 path.string() + " of type " + type};
        return nullptr;
    }

    using CreateT = std::function<File *(Target &target,
                                         std::filesystem::path path,
                                         std::filesystem::path from)>;

    void registerFunction(std::filesystem::path extension, CreateT f) {
        _createFunctions[extension] = f;
    }

    File *find(std::filesystem::path path) {
        return _index->find(path);
    }

    // Create temporary file that can be removed after compilation
    File *createIntermediateFile(std::filesystem::path path) {
        auto file = std::make_unique<File>(path, _paths.cache / path);
        _files.push_back(file.get());
        _index->files.push_back(std::move(file));
        return _index->files.back().get();
    }

    File *createOutputFile(std::filesystem::path path) {
        auto file = std::make_unique<File>(path, _paths.out / path);
        _files.push_back(file.get());
        _index->files.push_back(std::move(file));
        return _index->files.back().get();
    }

    friend void to_json(nlohmann::json &j, const Target &t) {
        j = nlohmann::json{
            {"files", t._files},
            {"name", t._name},
        };
    }

    void addObject(File *file) {
        _output->dependencies.push_back(file);
    }

    // Root node of target
    File *output() {
        return _output;
    }

    const Index &index() const {
        return *_index;
    }

    File *findSystemFile(std::filesystem::path path) {
        return _index->findSystemFile(path);
    }

    void includes(std::vector<std::filesystem::path> includes) {
        _includes = std::move(includes);
    }

    const auto &includes() const {
        return _includes;
    }

    // Files used by this target
    const auto &files() const {
        return _files;
    }

private:
    Index *_index;

    std::vector<File *> _files;
    std::vector<std::filesystem::path> _includes;
    std::unordered_map<std::string, CreateT> _createFunctions;
    std::string _name = "main";

    File *_output = nullptr;
    const BuildPaths &_paths;
};
