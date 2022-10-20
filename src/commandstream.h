#pragma once

#include <iostream>
#include <sstream>

inline bool dryRun = false;

struct Command : public std::ostringstream {
    void run() {
        std::cout << str() << std::endl;
        if (dryRun) {
        }
        else {
            if (std::system(str().c_str())) {
                throw std::runtime_error{"failed with command: " + str()};
            }
        }
    }
};
