#pragma once

#include <iostream>
#include <sstream>

inline bool dryRun = false;

struct Command : public std::ostringstream {
    void run() {
        if (dryRun) {
            std::cout << str() << std::endl;
        }
        else {
            if (std::system(str().c_str())) {
                throw std::runtime_error{"failed with command: " + str()};
            }
        }
    }
};
