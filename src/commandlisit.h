#pragma once

#include "file.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// inline bool dryRun = false;

// struct CommandStream : public std::ostringstream {
//     CommandStream(struct CommandList &list, const File &file)
//         : list{list}
//         , file{file} {}

//    ~CommandStream();

//    //    void run() {
//    //        std::cout << str() << std::endl;
//    //        if (dryRun) {
//    //        }
//    //        else {
//    //            if (std::system(str().c_str())) {
//    //                throw std::runtime_error{"failed with command: " +
//    str()};
//    //            }
//    //        }
//    //    }

//    CommandList &list;
//    const File &file;
//};

struct CommandList {
    struct Command {
        std::string name;
        const File &file;
    };

    std::vector<Command> commands;

    //    CommandStream operator()(const File &file) {
    //        return CommandStream{*this, file};
    //    }
};

// inline CommandStream::~CommandStream() {
//     list.commands.push_back(CommandList::Command{
//         .name = str(),
//         .file = file,
//     });
// }
