//--------------------------------------
// uiWindow
//--------------------------------------
// A simple non-virtual class that can be stored in PROGMEM

#pragma once

#include "Grbl_MinUI.h"
#include "myTFT.h"

#ifdef WITH_APPLICATION

    // cast to const void *

    #define V(x)        ((const void *)(x))

    // function typedef prototypes for posterities sake
    // typedef int (*intFunction)();
    // typedef void (*buttonCallback)(uiWindow *, const uiElement*) ;

    typedef const char *(*constCharStarMethod)();

    // uiElement types OR'd into the ID_TYPE

    #define ID_TYPE_TEXT     0x1000      // param is const char *
    #define ID_TYPE_BUTTON   0x2000      // button with background
    #define ID_TYPE_MUTABLE  0x4000      // param is pointer to a uiMutable
    #define ID_TYPE_TEXT_FN  0x8000      // param is const char * method

    #define ID_TYPE_ALL      0xf000      // things that get drawn by the system


    #define WIN_STATE_NONE      0x0000
    #define WIN_STATE_VALID     0x0001

    #define COLOR_BUTTON_DISABLED     COLOR_LIGHTGREY
    #define COLOR_BUTTON_HIDDEN       COLOR_BLACK
    #define COLOR_BUTTON_BG_PRESSED   COLOR_CYAN
    #define COLOR_BUTTON_FG_PRESSED   COLOR_BLACK

    class uiMutable
    {
        public:

            const char  *text;
            uint16_t     bg;
            uint16_t     fg;
            FontType     font;
    };


    class uiElement
    {
        public:

            const int16_t       id_type;
            const int16_t       x;
            const int16_t       y;
            const int16_t       w;
            const int16_t       h;
            const void         *param;
            const uint16_t      bg;
            const uint16_t      fg;
            const FontType      font;
            const JustifyType   just;
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

#endif  // WITH_APPLICATION
