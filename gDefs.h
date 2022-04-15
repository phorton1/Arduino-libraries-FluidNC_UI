#pragma once

#define UI_VERSION       "prh 1.1.0"
#define UI_VERSION_DATE  "2021-07-15"

#define UI_WITH_MESH
    // This should match the usage of the FluidNC_extension
    // Mesh.h as defined in your machine

// UI axes definitions
// UI only supports a simple 3 axis machine at this time

#define UI_NUM_AXES      3
#define UI_AXIS_X        0
#define UI_AXIS_Y        1
#define UI_AXIS_Z        2

// definitions common to FluidNC_UI
// This needs to be rethought for 480x320 screens

#if 0
    #define UI_SCREEN_WIDTH         480
    #define UI_SCREEN_HEIGHT        320
#else
    #define UI_SCREEN_WIDTH         320
    #define UI_SCREEN_HEIGHT        240
#endif

#define UI_TOP_MARGIN           35
#define UI_BOTTOM_MARGIN        34
#define UI_MAIN_BUTTON_WIDTH    80
#define UI_INDICATOR_WIDTH      28

#define UI_CONTENT_HEIGHT       (UI_SCREEN_HEIGHT - UI_TOP_MARGIN - UI_BOTTOM_MARGIN)

#define UI_MAX_TITLE            30          // maximum length of system title
#define UI_MAX_BUTTON            7          // maximum length of a mutable button
