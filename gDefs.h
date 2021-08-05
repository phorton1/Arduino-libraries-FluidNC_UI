#pragma once

#define UI_VERSION       "prh 1.1.0"
#define UI_VERSION_DATE  "2021-07-15"

// Only one of these should be defined and the driver
// must match the one set in TFT_eSPI/prh_Setup.h.
// For the time being the only difference is the default
// calibration data used if SPIFFS tft_data.txt is not found.
// Remember, you can press 5 times on any unused portion of the
// screen to invoke TFT calibration.

// #define TFT_2_POINT_8_INCH_ILI9341
#define TFT_3_POINT_2_INCH_ILI9341
// #define TFT_3_POINT_5_INCH_ILI9488

// Debugging defines

#define WITH_INIT_UI
    // can be commented out for memory footprint measurements
    // required for all practical purposes
#define WITH_TFT
    // can be commented out for memory footprint measurements
    // comment out for minimal library which just includes g_debug() to serial
    // required for all practical purposes
#define WITH_APPLICATION
    // can be commented out for memory footprint measurements
    // Comment this out to run a bare-bones library with just the TFT
    // required for all practical purposes
#define WITH_GRBL
    // Comment this out to run the UI without linking to Grbl_Esp32.
    // Must be commented out to run _testGrblUI.ino
    // Should be commented in for "production"


#ifdef WITH_APPLICATION

    #define UI_NUM_AXES      3
    #define UI_AXIS_X        0
    #define UI_AXIS_Y        1
    #define UI_AXIS_Z        2

    // definitions common to Grbl_TouchUI

    #ifdef TFT_3_POINT_5_INCH_ILI9488
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

    #define IND_STATE_NONE         0x00
    #define IND_STATE_ENABLED      0x01
    #define IND_STATE_READY        0x02
    #define IND_STATE_ACTIVE       0x04
    #define IND_STATE_ERROR        0x08
    #define IND_STATE_ALL          0x0f

    // Denormalized define of vMachine SDCard CS pin.
    // If not linked to GRBL, wherein the vMachine
    // initializes the SD Card at startup, somebody
    // needs to *at least* set the pin HIGH or else
    // the Touch portion of the TFT does not work,
    // presumably due to the SDCard's non-standard
    // use of the CS bus.

    #ifndef V_SDCARD_CS
        #define V_SDCARD_CS   GPIO_NUM_21
    #endif


#endif  // WITH_APPLICATION
