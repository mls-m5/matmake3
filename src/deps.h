#pragma once

#include <filesystem>
#include <string>
#include <vector>

std::vector<std::string> parseModuleDeps(std::filesystem::path path);
