#pragma once

#include "buildpaths.h"
#include "commandlisit.h"

/// Write compile_commands.json for clang tools
void writeCompilationDatabase(const BuildPaths &paths, const CommandList &list);
