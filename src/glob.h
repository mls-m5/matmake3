#pragma once

#include <regex>

/// Wildcard file search, translate "glob" expression to regex
std::regex glob2Regex(std::string str);
