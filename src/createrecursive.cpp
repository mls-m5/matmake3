#include "createrecursive.h"
#include "buildpaths.h"
#include "deps.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

File *requestPcm(Target &target, std::string dep) {
    auto name = dep;
    auto type = ".pcm";
    if (dep.front() == '<' || dep.front() == '"') {
        auto path = std::filesystem::path{name.substr(1, name.size() - 2)}
                        .replace_extension(".pcm");
        name = path.string();
        type = ".h.pcm";
    }
    else {
        name += ".pcm";
    }
    return target.requestObject(name, dep, type);
}

File *createRegularObjectFile(Target &target,
                              std::filesystem::path path,
                              File &src) {
    auto deps = parseModuleDeps(src.path);

    path = src.sameDir(path);

    auto file = target.createIntermediateFile(path);
    for (auto &dep : deps) {
        //        auto module = dep + ".pcm";
        file->dependencies.push_back(requestPcm(target, dep));
    }

    file->src = &src;

    return file;
}

File *createModuleObjectFile(Target &target,
                             std::filesystem::path path,
                             File &src) {

    path = src.sameDir(path);

    auto file = target.createIntermediateFile(path);
    file->src = &src;

    return file;
}

File *createObjectFile(Target &target,
                       std::filesystem::path path,
                       std::filesystem::path from) {
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

File *createPcmFile(Target &target,
                    std::filesystem::path path,
                    std::filesystem::path from) {
    auto srcPath = path;
    srcPath.replace_extension(".cppm");
    auto src = target.find(srcPath);

    if (!src) {
        throw std::runtime_error{
            "could not find source for pcm (c++-module) file " +
            srcPath.string()};
    }

    path = src->sameDir(path);

    auto file = target.createIntermediateFile(path);
    file->src = src;

    {
        auto deps = parseModuleDeps(src->path);
        for (auto &dep : deps) {
            file->dependencies.push_back(requestPcm(target, dep));
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

File *createPcmHeaderFile(Target &target,
                          std::filesystem::path path,
                          std::filesystem::path from) {
    bool isSystemHeader = false;

    if (from.string().front() == '<') {
        isSystemHeader = true;
    }
    else if (from.string().front() != '\"') {
        throw std::runtime_error{"not a header: " + from.string()};
    }

    auto srcPath = from.string();
    srcPath = srcPath.substr(1, srcPath.size() - 2);
    auto src = [isSystemHeader, srcPath, &target] {
        if (isSystemHeader) {
            return target.findSystemFile(srcPath);
        }

        return target.find(srcPath);
    }();

    if (!src) {
        throw std::runtime_error{
            "could not find source for pcm (c++-module) file " + srcPath};
    }

    path = src->sameDir(path);

    auto file = target.createIntermediateFile(path);
    file->buildType = isSystemHeader ? "sys.h.pcm" : ".h.pcm";
    file->src = src;
    return file;
}

} // namespace

std::unique_ptr<Target> createRecursive(Index &index, const BuildPaths &paths) {
    auto target = std::make_unique<Target>("main", index, paths);

    target->registerFunction(".o", createObjectFile);
    target->registerFunction(".pcm", createPcmFile);
    target->registerFunction(".h.pcm", createPcmHeaderFile);

    for (auto src : index.findAll(".cpp")) {
        auto objSrc = src->path;
        objSrc.replace_extension(".o");
        auto obj = target->requestObject(objSrc, src->path);
        target->addObject(obj);
    }

    {
        auto includes = std::vector<std::filesystem::path>{};
        for (auto it : std::filesystem::recursive_directory_iterator{"."}) {
            if (it.path().filename() == "include") {
                includes.push_back(std::filesystem::relative(it.path(), "."));
            }
        }
        target->includes(std::move(includes));
    }

    target->output()->buildType = "exe";

    std::cout << std::setw(2)
              << nlohmann::json{{"index", index}, {"target", *target},}
              << std::endl;

    return target;
}
