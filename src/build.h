#pragma once

#include "target.h"

// Crude single threaded build system
void build(Target &target);

void createBuildPaths(const Index &index);
