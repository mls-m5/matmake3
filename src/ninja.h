#pragma once

#include "buildpaths.h"
#include "commandlisit.h"

void writeNinjaFile(const BuildPaths &paths, const CommandList &list);
void runNinjaFile(const BuildPaths &);
