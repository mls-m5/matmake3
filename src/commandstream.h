#pragma once

#include <iostream>
#include <sstream>

inline bool dryRun = true;

struct Command : public std::ostringstream {
    ~Command() {
        if (dryRun) {
            std::cout << str() << std::endl;
        }
        else {
            if (!std::system(str().c_str())) {
                std::runtime_error{"failed commando: " + str()};
            }
        }
    }
};
