#pragma once

#include "buildpaths.h"
#include "commandlisit.h"
#include "settings.h"

void writeNinjaFile(const BuildPaths &paths, const CommandList &list);
void runNinjaFile(const Settings &settings);
void ninjaClean(const BuildPaths &paths);
