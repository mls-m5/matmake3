#pragma once

#include "buildpaths.h"
#include "commandlisit.h"
#include "settings.h"

/// Functions for interacting with the ninja build system

void writeNinjaFile(const BuildPaths &paths, const CommandList &list);
void runNinjaFile(const Settings &settings);
void ninjaClean(const BuildPaths &paths);
