//--------------------------------------
// uiWindow
//--------------------------------------
// A simple non-virtual class that can be stored in PROGMEM

#pragma once

#include "FluidNC_UI.h"
#include "myTFT.h"

// cast to const void *

#define V(x)        ((const void *)(x))

// function typedef prototypes for posterities sake
// typedef int (*intFunction)();
// typedef void (*buttonCallback)(uiWindow *, const uiElement*) ;

typedef const char *(*constCharStarMethod)();

// uiElement types OR'd into the ID_TYPE

#define ID_TYPE_TEXT        0x0100      // param is const char *
#define ID_TYPE_BUTTON      0x0200      // button with background
#define ID_TYPE_MUTABLE     0x1000      // param is pointer to a uiMutable
#define ID_TYPE_DUAL        0x2000      // param is pointer to a uiDualElement
#define ID_TYPE_NO_BORDER   0x0400      // suppress outline on buttons
#define ID_TYPE_ALL         0xff00      // things that get drawn by the system

#define COLOR_BUTTON_DISABLED     COLOR_DARKGREY
#define COLOR_BUTTON_HIDDEN       COLOR_BLACK
#define COLOR_BUTTON_BG_PRESSED   COLOR_CYAN
#define COLOR_BUTTON_FG_PRESSED   COLOR_BLACK

#define ELEMENT_TYPE_STR        0x0000
#define ELEMENT_TYPE_INT        0x0001
#define ELEMENT_TYPE_FLOAT      0x0002




class uiElement     // the most basic element of a window
{                   // these are CONST in usage (and take NO dynamic memory)
    public:

        const int16_t       id_type;        // it's id and 'type' combined
        const int16_t       x;              // the rectangle
        const int16_t       y;
        const int16_t       w;
        const int16_t       h;
        const void         *param;          // a pointer to a string, integer, or another kind of element
        const uint16_t      bg;             // background color, etc
        const uint16_t      fg;
        const FontType      font;
        const JustifyType   just;
};




class uiMutable     // a mutable element can use a little memory to provide
{
    public:
        const char  *text;      // a variable pointer to some text
        uint16_t     bg;        // and variable display properties
        uint16_t     fg;        // that can change at runtime
        FontType     font;
};


class uiDualElement // a dual element consists of a label part and a value part
{
    public:
        uint16_t     type;              // the type of the value
        uint16_t     bg;                // variable background color of the whole thing
        int16_t      label_width;       // how much of it is the label (from left)
        int16_t      value_width;       // and how much is the value (from right)
        const char  *label;             // a pointer to the label
        const void  *value;             // a pointer to a value given by the type
        FontType     value_font;

};





class uiWindow
{
    public:

        uiWindow(const uiElement *wins, uint16_t num_elements) :
            m_elements(wins),
            m_num_elements(num_elements) {}

        virtual void begin()
        {
            tft.fillRect(0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_SCREEN_HEIGHT-UI_TOP_MARGIN-UI_BOTTOM_MARGIN,COLOR_BLACK);
            drawTypedElements();
        }

        virtual void update() {}
        virtual void onButton(const uiElement *ele, bool pressed) {}
        virtual bool isModal() const { return false; }
        virtual const char *getMenuLabel() const  { return ""; }

        // draw all typed elements
        // DO NOT CALL drawTypedElement except on "typed" elements
        // OR one with a V(const char *) param member !!!

        void drawTypedElements() const;
        void drawTypedElement(const uiElement *ele, bool pressed = false) const;

        // touch and button handler handler

        static void updateTouch();
        bool hitTest();

    protected:

        uint16_t         m_num_elements;
        const uiElement *m_elements;

        static bool             g_pressed;
        static int              g_press_x;
        static int              g_press_y;
        static uint32_t         g_debounce_time;
        static const uiElement *g_win_pressed;
        static uiWindow        *g_window_pressed;

};
