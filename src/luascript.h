#pragma once

#include "buildpaths.h"
#include "index.h"
#include "target.h"

bool hasBuildScript(const BuildPaths &paths);

void runBuildScript(const BuildPaths &paths,
                    const Index &index,
                    Target &target);
