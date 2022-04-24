#pragma once

#include <array>
#include <filesystem>

bool isHeaderFile(const std::filesystem::path &path);

bool isSourceFile(const std::filesystem::path &path);

// Should be part of build
bool shouldInclude(const std::filesystem::path &path);
