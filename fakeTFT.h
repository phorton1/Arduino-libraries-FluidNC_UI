#pragma once

// #define USE_FAKE_TFT

// Uncomment above to use this partial re-implementation of TFT_eSPI.
// It is not even close, but it allowed me to debug the conflict with
// the SDCard upload in FluidNC webServer.cpp, so I am keeping it.
// I got as far as doing fillRect working (in the wrong rotation)


#ifdef USE_FAKE_TFT

    #include <Arduino.h>
    // #include <Print.h>
    // #include <SPI.h>

    typedef struct { // Data stored PER GLYPH
        uint32_t bitmapOffset;     // Pointer into GFXfont->bitmap
        uint8_t  width, height;    // Bitmap dimensions in pixels
        uint8_t  xAdvance;         // Distance to advance cursor (x axis)
        int8_t   xOffset, yOffset; // Dist from cursor pos to UL corner
    } GFXglyph;

    typedef struct { // Data stored for FONT AS A WHOLE:
        uint8_t  *bitmap;      // Glyph bitmaps, concatenated
        GFXglyph *glyph;       // Glyph array
        uint16_t  first, last; // ASCII extents
        uint8_t   yAdvance;    // Newline distance (y axis)
    } GFXfont;

    //These enumerate the text plotting alignment (reference datum point)
    #define TL_DATUM 0 // Top left (default)
    #define TC_DATUM 1 // Top centre
    #define TR_DATUM 2 // Top right
    #define ML_DATUM 3 // Middle left
    #define CL_DATUM 3 // Centre left, same as above
    #define MC_DATUM 4 // Middle centre
    #define CC_DATUM 4 // Centre centre, same as above
    #define MR_DATUM 5 // Middle right
    #define CR_DATUM 5 // Centre right, same as above
    #define BL_DATUM 6 // Bottom left
    #define BC_DATUM 7 // Bottom centre
    #define BR_DATUM 8 // Bottom right
    #define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
    #define C_BASELINE 10 // Centre character baseline
    #define R_BASELINE 11 // Right character baseline

    #define TFT_BLACK       0x0000      /*   0,   0,   0 */
    #define TFT_NAVY        0x000F      /*   0,   0, 128 */
    #define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
    #define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
    #define TFT_MAROON      0x7800      /* 128,   0,   0 */
    #define TFT_PURPLE      0x780F      /* 128,   0, 128 */
    #define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
    #define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
    #define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
    #define TFT_BLUE        0x001F      /*   0,   0, 255 */
    #define TFT_GREEN       0x07E0      /*   0, 255,   0 */
    #define TFT_CYAN        0x07FF      /*   0, 255, 255 */
    #define TFT_RED         0xF800      /* 255,   0,   0 */
    #define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
    #define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
    #define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
    #define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
    #define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
    #define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
    #define TFT_BROWN       0x9A60      /* 150,  75,   0 */
    #define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
    #define TFT_SILVER      0xC618      /* 192, 192, 192 */
    #define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
    #define TFT_VIOLET      0x915C      /* 180,  46, 226 */


    class TFT_eSPI  //  : public Print
    {
        public:

            TFT_eSPI() {}

            void init();

            void setRotation(uint8_t r) {}
            void setTouch(uint16_t *data) {}

            void fillScreen(uint32_t color);
            void fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
            void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color) {}
            void fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color) {}

            void setTextDatum(uint8_t datum)                                {}
            void setTextColor(uint16_t fgcolor, uint16_t bgcolor)           {}
            void setTextPadding(uint16_t x_width)                           {}
            int16_t drawString(const char *string, int32_t x, int32_t y)    { return 0; }
            void setFreeFont(const GFXfont *f = NULL)                       {}
            void setViewport(int32_t x, int32_t y, int32_t w, int32_t h)    {}
            void resetViewport(void) {}

            bool calibrateTouch(uint16_t *data, uint32_t color_fg, uint32_t color_bg, uint8_t size, uint32_t timeout=0) { return false; }
            uint8_t  getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600)   { return 0; }

            uint16_t readPixel(int32_t x, int32_t y) { return 0; }
            uint32_t color16to24(uint16_t color565)  { return 0; }
    };

#endif  // USE_FAKE_TFT
