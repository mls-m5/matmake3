#include "index.h"
#include <regex>

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
