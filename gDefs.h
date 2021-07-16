#pragma once

#define UI_NUM_AXES      3
#define UI_AXIS_X        0
#define UI_AXIS_Y        1
#define UI_AXIS_Z        2

// definitions common to Grbl_TouchUI

#define UI_VERSION       "prh 1.1.0"
#define UI_VERSION_DATE  "2021-07-15"

#define UI_SCREEN_WIDTH         320
#define UI_SCREEN_HEIGHT        240

#define UI_TOP_MARGIN           35
#define UI_BOTTOM_MARGIN        35
#define UI_MAIN_BUTTON_WIDTH    80
#define UI_INDICATOR_WIDTH      28

#define IND_STATE_ENABLED      0x0001
#define IND_STATE_READY        0x0002
#define IND_STATE_ACTIVE       0x0004
#define IND_STATE_ERROR        0x0008
#define IND_STATE_ALL          0x000f

// include TFT_eSPI for everybody

#include <TFT_eSPI.h>

extern TFT_eSPI tft;    // in grblTouchUI.cpp

extern const GFXfont Dialog_plain_14;
extern const GFXfont Dialog_plain_16;
extern const GFXfont Dialog_plain_18;
extern const GFXfont Dialog_plain_22;
extern const GFXfont Roboto_Mono_Bold_14;
