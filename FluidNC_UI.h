//--------------------------------
// FluidNC_UI.h
//--------------------------------

#pragma once

#include "gDefs.h"
#include <FluidDebug.h> // FluidNC_extensions

// Thus far all machines (vMachine, cnc3018_v1, cnc3018_v2, and cnc20mm) have all used 3.2" ILI9431 drivers.
// The only thing that has changed is that cnc20mm will use rotation(1) versus the previous using rotation(3)
// This API is somewhat fleshed out, the implementation is not.

extern void FluidNC_UI_init(
    const char *driver = "ILI9431_32",
        // ILI9431_28,      // 2.8" 320X240  rotation(3) only
        // ILI9431_32,      // 3.2" 320X240  rotation(1) and rotation(3)
        // ILI9486_35,      // 3.5" 480x320 (TODO)
        // ILI9488_40,      // 4.0" 480x320 (TODO)
    int rotation = 3 );

