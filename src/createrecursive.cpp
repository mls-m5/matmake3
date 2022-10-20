#include "createrecursive.h"
#include "deps.h"
#include <iostream>

namespace {

File *createRegularObjectFile(Target &target,
                              std::filesystem::path path,
                              File &src) {
    auto deps = parseModuleDeps(src.path);

    path = src.sameDir(path);

    auto file = target.createIntermediateFile(path);
    for (auto &dep : deps) {
        auto module = dep + ".pcm";
        file->dependencies.push_back(target.requestObject(module));
    }

    return file;
}

File *createModuleObjectFile(Target &target,
                             std::filesystem::path path,
                             File &src) {

    path = src.sameDir(path);

    auto file = target.createIntermediateFile(path);
    file->dependencies.push_back(&src);

    return file;
}

File *createObjectFile(Target &target, std::filesystem::path path) {
    {
        auto srcPath = path;
        srcPath.replace_extension(".cppm");
        auto src = target.find(srcPath);

        if (src) {
            return createModuleObjectFile(target, path, *src);
        }
    }

    auto srcPath = path;
    srcPath.replace_extension(".cpp");
    auto src = target.find(srcPath);

    path = src->sameDir(path);

    if (src) {
        return createRegularObjectFile(target, path, *src);
    }

    return nullptr;
}

File *createPcmFile(Target &target, std::filesystem::path path) {
    auto srcPath = path;
    srcPath.replace_extension(".cppm");
    auto src = target.find(srcPath);

    if (!src) {
        throw std::runtime_error{
            "could not find source for pcm (c++-module) file"};
    }

    path = src->sameDir(path);

    auto file = target.createIntermediateFile(path);
    file->dependencies.push_back(src);

    {
        auto deps = parseModuleDeps(src->path);
        for (auto &dep : deps) {
            file->dependencies.push_back(target.requestObject(dep + ".pcm"));
        }
    }

    {
        auto objPath = path;
        objPath.replace_extension(".o");
        auto objFile = target.find(objPath);
        if (objFile) {
            throw std::runtime_error{
                objPath.string() +
                " does already exist, refusing to create duplicate"};
        }

        objFile = createModuleObjectFile(target, objPath, *file);
        target.addObject(objFile);
    }

    return file;
}

} // namespace

std::unique_ptr<Target> createRecursive(Index &index) {
    auto target = std::make_unique<Target>("main", index);

    target->registerFunction(".o", createObjectFile);
    target->registerFunction(".pcm", createPcmFile);

    auto main = target->requestObject("main.o");
    target->addObject(main);

    std::cout << std::setw(2) << nlohmann::json{{"index", index},{"target",*target},} << std::endl;

    return target;
}
