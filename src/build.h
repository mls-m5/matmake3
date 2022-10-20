#pragma once

#include "settings.h"
#include "target.h"

// Crude single threaded build system
void build(Target &target, const Settings &settings);

void createBuildPaths(const Index &index);
