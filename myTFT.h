//-------------------------------------------------
// definition and slight extensions to TFT_eSPI
//-------------------------------------------------

#pragma once

#include "fakeTFT.h"

#ifndef USE_FAKE_TFT
    #include "TFT_eSPI.h"
#endif


// Colors denormalized and some renamed from TFT_eSPI.h

#define COLOR_BLACK       0x0000      //    0,   0,   0
#define COLOR_DARKBLUE    0x000F      //    0,   0, 128
#define COLOR_DARKGREEN   0x03E0      //    0, 128,   0
#define COLOR_DARKCYAN    0x03EF      //    0, 128, 128
#define COLOR_DARKRED     0x7800      //  128,   0,   0
#define COLOR_PURPLE      0x780F      //  128,   0, 128
#define COLOR_OLIVE       0x7BE0      //  128, 128,   0
#define COLOR_LIGHTGREY   0xD69A      //  211, 211, 211
#define COLOR_DARKGREY    0x7BEF      //  128, 128, 128
#define COLOR_BLUE        0x001F      //    0,   0, 255
#define COLOR_GREEN       0x07E0      //    0, 255,   0
#define COLOR_CYAN        0x07FF      //    0, 255, 255
#define COLOR_RED         0xF800      //  255,   0,   0
#define COLOR_MAGENTA     0xF81F      //  255,   0, 255
#define COLOR_YELLOW      0xFFE0      //  255, 255,   0
#define COLOR_WHITE       0xFFFF      //  255, 255, 255
#define COLOR_ORANGE      0xFDA0      //  255, 180,   0
#define COLOR_GREENYELLOW 0xB7E0      //  180, 255,   0
#define COLOR_PINK        0xFE19      //  255, 192, 203   Lighter pink, was 0xFC9F
#define COLOR_BROWN       0x9A60      //  150,  75,   0
#define COLOR_GOLD        0xFEA0      //  255, 215,   0
#define COLOR_SILVER      0xC618      //  192, 192, 192
#define COLOR_SKYBLUE     0x867D      //  135, 206, 235
#define COLOR_VIOLET      0x915C      //  180,  46, 226

typedef enum JustifyType
{
    JUST_CENTER,
    JUST_LEFT,           // mid
    JUST_RIGHT,
    JUST_LEFT_TOP,
    JUST_CENTER_TOP,
    JUST_RIGHT_TOP,
    JUST_LEFT_BOTTOM,
    JUST_CENTER_BOTTOM,
    JUST_RIGHT_BOTTOM,
};


typedef enum FontType
{
    FONT_TINY,
    FONT_SMALL,
    FONT_NORMAL,
    FONT_BIG,
    FONT_MONO,
    FONT_SYMBOL,
};


//--------------------------
// extensions
//--------------------------

extern void drawText(           // draw clipped and justified text
        const char *text,
        JustifyType just,
        FontType font,
        int16_t x, int16_t y, int16_t w, int16_t h, // absolute bounding box
        uint16_t fg,
        uint16_t bg);


//---------------------------
// global object
//---------------------------


extern TFT_eSPI tft;

extern void init_my_tft();
extern void calibrate_my_tft();
