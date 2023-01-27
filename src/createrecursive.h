#pragma once
#include "buildpaths.h"
#include "target.h"

//! Load in files and setup targets based on what is found
std::unique_ptr<Target> createRecursive(Index &index, const BuildPaths &paths);
