#pragma once

#include "buildcontext.h"
#include "settings.h"
#include "target.h"

// Crude single threaded build system
void build(Target &target, const Settings &settings);

void createBuildPaths(const Target &target, BuildContext &context);
