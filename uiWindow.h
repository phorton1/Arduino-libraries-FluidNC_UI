//-------------------------------------------------
// Generic OOP Windowing System based on TFT_eSPI
//-------------------------------------------------
// Does not use any dynamic memory.
// Allows for static construction of entire application

#pragma once

#include "uiRect.h"
#include "gDefs.h"      // include gDefs.h for almost everybody

#define WIN_STATE_NONE   0x0000
#define WIN_STATE_VALID  0x0001
#define WIN_STATE_HIDDEN 0x0002

// Colors denormalized and some renamed from TFT_eSPI.h

#define COLOR_BLACK       0x0000      //    0,   0,   0
#define COLOR_DARKBLUE    0x000F      //    0,   0, 128
#define COLOR_DARKGREEN   0x03E0      //    0, 128,   0
#define COLOR_DARKCYAN    0x03EF      //    0, 128, 128
#define COLOR_MAROON      0x7800      //  128,   0,   0
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


class uiWindow
{
    public:

        uiWindow()  {}      // application only

        uiWindow(const uiWindow &parent,
                 s16 x, s16 y, s16 w, s16 h,
                 uint16_t bg,
                 uint16_t fg,
                 uint16_t state=WIN_STATE_NONE) :  // typical ctor
            m_rect(x,y,w,h),
            m_bg(bg),
            m_fg(fg),
            m_state(state)
        {
            m_rect.makeRelative(parent.m_rect);
        }

        virtual void draw();
        virtual void update() {}

        void setState(uint16_t state)     { m_state = state; }
        void addState(uint16_t state)     { m_state |= state; }
        void clearState(uint16_t state)   { m_state &= ~state; }
        bool hasState(uint16_t state)     { return m_state & state; }


    protected:

        uiRect m_rect;        // absolute
        uint16_t m_bg;
        uint16_t m_fg;
        uint16_t m_state = WIN_STATE_NONE;
};


typedef enum JustifyType
{
    JUST_LEFT,           // mid
    JUST_CENTER,
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
};

class uiStaticText : public uiWindow
{
    public:

        uiStaticText(const uiWindow &parent,
                     const char *text,
                     JustifyType just,
                     FontType    font,
                     s16 x, s16 y, s16 w, s16 h,
                     uint16_t bg,
                     uint16_t fg,
                     uint16_t state=WIN_STATE_NONE) :  // typical ctor
            uiWindow(parent,x,y,w,h,bg,fg,state),
            m_text(text),
            m_just(just),
            m_font(font)
        {
        }

        virtual void draw();

    protected:

        const char *m_text;
        JustifyType m_just;
        FontType    m_font;

};
