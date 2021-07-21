//--------------------------------------
// The Grbl_MinUI application class
//--------------------------------------

#pragma once

#include "uiWindow.h"
#include "gStatus.h"

#define CONFIRM_COMMAND_RESET   0x0001
#define CONFIRM_COMMAND_REBOOT  0x0002


typedef struct appLast_t
    // A structure that holds previous state of
    // application for change detection.
{
    // system change detection

    uint32_t       mem_avail;
    uint32_t       min_avail;

    // gStatus change detection

    uint8_t        wifi_state;
    grbl_State_t   sys_state;
    grbl_SDState_t sd_state;
    float          pct;
    float          pos[UI_NUM_AXES * 2];

    // gApplication change detection

    int16_t        prog_w;
    uint16_t       prog_color;
    char           app_title[UI_MAX_TITLE + 1];

};



class gApplication : public uiWindow
{
    public:

        gApplication();

        virtual void begin() override;
        virtual void update() override;

        void setTitle(const char *text);
        void setCurWindow(uiWindow *ele);

        void confirmCommand(uint16_t command);
        void endConfirm(uint16_t rslt);

    private:

        bool draw_needed;

        uiWindow *cur_window;
        uiWindow *prev_window;
        uint16_t pending_command;

        // progress bar

        int16_t  prog_x;
        int16_t  prog_w;
        uint16_t prog_color;

        // previous state

        appLast_t last;

        //-----------------------------
        // methods
        //-----------------------------

        uint16_t indColor(uint8_t ind_state);
        void doText(const uiElement *ele, const char *text);
        void doTextProgress(const uiElement *ele, const char *text, bool changed);

        // dispatched functions

        void doWin(const uiElement *ele);
        void doAppButton(const uiElement *ele);
        void doAppTitle(const uiElement *ele);
        void doSDIndicator(const uiElement *ele);
        void doWifiIndicator(const uiElement *ele);
        void doMachinePosition(const uiElement *ele);
        void doWorkPosition(const uiElement *ele);
        void doSysState(const uiElement *ele);
        void doMemAvail(const uiElement *ele);
        void doJobProgress(const uiElement *ele);

};


extern gApplication the_app;
