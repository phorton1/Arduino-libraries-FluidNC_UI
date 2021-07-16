//--------------------------------------
// GRBL application specific classes
//--------------------------------------
// gApplication, gTitleBar, gContentArea, and gStatusBar

#pragma once

#include "uiWindow.h"
#include "gStatus.h"


class gTitleBar : public uiWindow
{
    public:

        gTitleBar();

        virtual void draw();

    private:

        uiStaticText m_main_button;
        uiStaticText m_title;

        // must be listed in initialization order!

        // gMainMenu  m_main_button;
        // uiLabel  m_title;
        // uiWindow m_indicator_area;
        // uiLabel  m_sdcard_indicator;
        // uiLabel  m_wifi_indicator;

        grbl_SDState_t m_last_sdcard_state = grbl_SDState_t::NotPresent;
        uint16_t m_last_wifi_state = 0;

};




class gStatusBar : public uiWindow
{
    public:

        gStatusBar();

    private:

        // uiStaticText m_text;

        // gStatusBarFloat mpos_x;
        // gStatusBarFloat mpos_y;
        // gStatusBarFloat mpos_z;
        // gStatusBarFloat wpos_x;
        // gStatusBarFloat wpos_y;
        // gStatusBarFloat wpos_z;
        //
        // uiLabel grblState;
        // uiLabel heapAvail;
        // uiLabel minHeapAvail;

        grbl_State_t m_last_grbl_state = grbl_State_t::Sleep;
        uint32_t m_heapAvail = 0;
        uint32_t m_minHeapAvail = 0;

};



class gApplication : public uiWindow
{
    public:

        gApplication();

        virtual void update();

        // void setTitle(const char *title);
        // void openWindow(int window_id);
        // int getCurWindowId()           { return m_cur_window_id; }

    private:

        gTitleBar    m_title_bar;
        gStatusBar   m_status_bar;

        // ui_main_mode_t m_last_mode = MAIN_MODE_NONE;
        // int          m_cur_window_id = 0;
        // uiWindow    *m_cur_window = NULL;
        //
        // void onModeChange();

};



//----------------------------------------------
// externs
//----------------------------------------------

extern gApplication the_app;
    // constructed in grblTouchUI.cpp
