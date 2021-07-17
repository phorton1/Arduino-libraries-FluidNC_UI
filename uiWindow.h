//-------------------------------------------------
// Generic OOP Windowing System based on TFT_eSPI
//-------------------------------------------------
// Does not use any dynamic memory.
// Allows for static construction of entire application

#pragma once

#include "uiRect.h"
#include "gDefs.h"      // include gDefs.h for almost everybody
#include "myTFT.h"      // for font types, etc

#define WIN_STATE_NONE   0x0000
#define WIN_STATE_VALID  0x0001
#define WIN_STATE_HIDDEN 0x0002


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
