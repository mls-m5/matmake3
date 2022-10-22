#pragma once
#include "buildpaths.h"
#include "target.h"

std::unique_ptr<Target> createRecursive(Index &index, const BuildPaths &paths);
