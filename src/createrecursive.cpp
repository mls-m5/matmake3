#include "createrecursive.h"

namespace {

File *createRegularObjectFile(Target &target,
                              std::filesystem::path path,
                              File &src) {
    return nullptr;
}

File *createModuleObjectFile(Target &target,
                             std::filesystem::path path,
                             File &src) {
    return nullptr;
}

File *createObjectFile(Target &target, std::filesystem::path path) {
    {
        auto srcPath = path;
        srcPath.replace_extension(".cpp");
        auto src = target.find(srcPath);

        if (src) {
            return createRegularObjectFile(target, path, *src);
        }
    }

    auto srcPath = path;
    srcPath.replace_extension(".cpp");
    auto src = target.find(srcPath);

    if (src) {
        return createRegularObjectFile(target, path, *src);
    }

    return nullptr;
}

} // namespace

std::unique_ptr<Target> createRecursive(Index &index) {
    auto target = std::make_unique<Target>(index);

    target->registerFunction(".o", createObjectFile);

    target->requestObject("main.o");

    return target;
}
