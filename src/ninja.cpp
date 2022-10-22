#include "ninja.h"
#include <fstream>
#include <iostream>

void printEscaped(std::ostream &stream, std::string_view str) {
    for (auto c : str) {
        if (c == ' ') {
            stream << "$ ";
        }
        else {
            stream << c;
        }
    }
}

void writeNinjaFile(const BuildContext &context, const CommandList &list) {

    std::filesystem::create_directories(context.paths().cache);
    auto file = std::ofstream{std::filesystem::path{context.paths().cache} /
                              "build.ninja"};

    std::cout << "printing build.ninja" << std::endl;

    file << "# Ninja file generated with matmake3\n\n";
    file << "rule run\n";
    file << "    command = $cmd\n\n";

    for (auto &command : list.commands()) {
        file << "build" << command.out << ": run ";
    }

    //    auto cxx = "${cxx} -x c++ ${in} ${modules} -o $out -c ${cxxflags}
    //    ${flags} "
    //               "${includes}";

    //    auto exe = "${cxx} ${in} -o ${out} ${ldflags} ${flags} ${includes}";

    //    auto printRule = [&file](std::string_view name, std::string_view rule)
    //    {
    //        file << "rule " << name << "\n"
    //             << "    command = " << rule << "\n\n";
    //    };

    //    printRule("cxx", cxx);
    //    printRule("exe", exe);

    //    auto set = [&file](std::string_view name, std::string_view value) {
    //        file << "    " << name << " = " << value << "\n";
    //    };

    //    auto srcToObj = [&cachePath](std::filesystem::path path) {
    //        path.replace_extension(".o");
    //        return cachePath / path;
    //    };

    //    for (auto &src : index.files) {
    //        if (isSourceFile(src->path)) {
    //            auto opath = srcToObj(src->path);

    //            file << "build " << opath.string() << ": cxx " <<
    //            src->path.string()
    //                 << "\n";
    //            set("cxx", "g++");
    //            file << "\n";
    //        }
    //    }

    //    auto parentPath =
    //        std::filesystem::absolute(std::filesystem::current_path()).filename();

    //    file << "build " << (outPath / parentPath).string() << ": exe ";

    //    for (auto &src : index.files) {
    //        if (!src->isSource()) {
    //            continue;
    //        }
    //        auto opath = srcToObj(src->path);
    //        printEscaped(file, opath.string());
    //        file << " ";
    //    }

    //    file << "\n";

    //    set("cxx", "g++");
}
