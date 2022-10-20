#include "deps.h"
#include <fstream>
#include <string_view>

using std::literals::operator""sv;

std::vector<std::string> parseModuleDeps(std::filesystem::path path) {
    // Todo: Maybe cache deps in intermediate dep file
    auto deps = std::vector<std::string>{};

    auto file = std::ifstream{path};

    auto importStr = "import "sv;
    auto exportImportStr = "export import "sv;

    auto findImport = [](const std::string &line,
                         std::string_view type) -> std::string {
        //        if (type.size() > line.size()) {
        //            return {};
        //        }
        if (line.rfind(type, 0) != 0) {
            return {};
        }
        auto dep = line.substr(type.size());
        if (dep.empty()) {
            return {};
        }
        if (dep.back() == ';') {
            dep.pop_back();
        }
        return dep;
    };

    for (std::string line; std::getline(file, line);) {
        if (line.empty()) {
            continue;
        }
        if (line.back() == '\r') {
            line.pop_back();
        }
        if (auto dep = findImport(line, importStr); !dep.empty()) {
            deps.push_back(dep);
            continue;
        }
        //        if (line.rfind(importStr, 0) == 0) {
        //            auto dep = line.substr(importStr.size());
        //            if (dep.empty()) {
        //                continue;
        //            }
        //            if (dep.back() == ';') {
        //                dep.pop_back();
        //            }
        //            deps.push_back(dep);
        //            continue;
        //        }
        if (line.rfind(exportImportStr, 0) == 0) {
            auto dep = line.substr(exportImportStr.size());
            if (dep.empty()) {
                continue;
            }
            if (dep.back() == ';') {
                dep.pop_back();
            }
            deps.push_back(dep);
            continue;
        }
    }

    return deps;
}
