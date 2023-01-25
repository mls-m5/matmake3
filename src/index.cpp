#include "index.h"
#include <regex>

Index::Index() {
    for (auto it = std::filesystem::recursive_directory_iterator{"."};
         it != decltype(it){};
         ++it) {
        if (it->path().filename() == "build") {
            it.disable_recursion_pending();
        }
        if (it->is_regular_file() && shouldInclude(it->path())) {
            auto rpath = std::filesystem::relative(it->path(), ".");
            files.push_back(std::make_unique<File>(rpath, rpath));
        }
    }
}

File *Index::find(std::filesystem::__cxx11::path path) const {
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

std::vector<File *> Index::findAll(std::string extension) const {
    auto files = std::vector<File *>{};
    for (auto &f : this->files) {
        if (f->path.extension() == extension) {
            files.push_back(f.get());
        }
    }

    return files;
}

std::vector<File *> Index::findGlob(std::string glob) const {
    auto files = std::vector<File *>{};
    auto re = glob2Regex(glob);
    for (auto &f : this->files) {
        if (std::regex_match(f->path.string(), re)) {
            files.push_back(f.get());
        }
    }

    return files;
}

File *Index::findSystemFile(std::filesystem::__cxx11::path path) {
    // TODO: Do something more generic
    auto libcHeaders = "/usr/lib/llvm-16/include/c++/v1/";

    if (auto file = find(path)) {
        return file;
    }

    auto fullPath = libcHeaders / path;
    if (std::filesystem::exists(fullPath)) {
        files.push_back(std::make_unique<File>(path, fullPath));
        return files.back().get();
    }

    fullPath = "/usr/include" / path;
    if (std::filesystem::exists(fullPath)) {
        files.push_back(std::make_unique<File>(path, fullPath));
        return files.back().get();
    }
    return nullptr;
}
