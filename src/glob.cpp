#include "glob.h"

std::regex glob2Regex(std::string str) {
    if (auto f = str.find("**"); f != std::string::npos) {
        str.replace(f, 2, ".*");
    }
    else if (auto f = str.find("*"); f != std::string::npos) {
        str.replace(f, 1, "[^\\/]*");
    }

    return std::regex{str};
}
