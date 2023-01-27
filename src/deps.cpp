#include "deps.h"
#include <fstream>
#include <string_view>

using std::literals::operator""sv;

std::vector<std::string> parseModuleDeps(std::filesystem::path path) {
    // TODO: Maybe cache deps in intermediate dep file

    auto strings = std::array{
        "import "sv,
        "module "sv,
        "export import "sv,
        "#include "sv,
    };

    auto findImport = [](const std::string &line,
                         std::string_view type) -> std::string {
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

    auto deps = std::vector<std::string>{};
    auto file = std::ifstream{path};

    for (std::string line; std::getline(file, line);) {
        if (line.empty()) {
            continue;
        }
        if (line.back() == '\r') {
            line.pop_back();
        }
        for (auto str : strings) {
            if (auto dep = findImport(line, str); !dep.empty()) {
                deps.push_back(dep);
                break;
            }
        }
    }

    return deps;
}
