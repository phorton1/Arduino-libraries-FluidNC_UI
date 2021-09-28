//--------------------------------------
// The FluidNC_UI application class
//--------------------------------------

#pragma once

#include "gDefs.h"
#include "uiWindow.h"
#include <gStatus.h>    // FluidNC_extensions


typedef struct appLast_t
    // A structure that holds previous state of
    // application for change detection.
{
    JobState       job_state;

    // system change detection

    uint32_t       mem_avail;
    uint32_t       min_avail;

    // gStatus change detection

    uint8_t        wifi_state;
    State          sys_state;
    SDState        sd_state;
    float          pct;
    float          pos[UI_NUM_AXES * 2];

    // gApplication change detection

    int16_t        prog_w;
    uint16_t       prog_color;
    char           app_title[UI_MAX_TITLE + 1];
    uiWindow      *window;

    uint8_t        status_mode;     // 0 == sys.state/memory  1==Feed Rate/zOffs

    float          feed_rate;
    float          feed_override;
    float          rapid_feed_override;
    float          spindle_override;

    #ifdef UI_WITH_MESH
        float      live_z;
        float      mesh_z;
    #endif

};



class gApplication : public uiWindow
{
    public:

        gApplication();

        virtual void begin() override;
        virtual void update() override;

        void setTitle(const char *text);
        void openWindow(uiWindow *win);
        void endModal();
        void setBaseWindow(uiWindow *win);
            // the menu sets the base window directly

        JobState getJobState()      { return job_state; }
        JobState getLastJobState()  { return last.job_state; }
            // UI Windows besides the application should NOT
            // use g_status directly.

        const char *getAppButtonText();

        void redrawAll()
            // needed layer violation so that uiWindow can
            // do the TFT calibration
        {
            draw_needed = true;
        }

        const char *getActiveFilename();
            // the last active filename, gotten whenever
            // a job is started

        void clearLastJobState()
            // for use in reset from dlgConfirm
            // to cause progress bar to clear correctly
            // and not get a toast when a job was in progress ..
        {
            job_state = JOB_NONE;
            last.job_state = JOB_NONE;
        }

        void suppressStatus()  { suppress_status = true; }

    private:

        State sys_state;
        SDState sd_state;
        JobState job_state;

        bool draw_needed;
        bool suppress_status;
        uint8_t status_mode;
        float feed_rate;
        #ifdef UI_WITH_MESH
            float      live_z;
            float      mesh_z;
        #endif


        // progress bar

        int16_t  prog_x;
        int16_t  prog_w;
        uint16_t prog_color;

        // previous state

        appLast_t last;

        //-----------------------------
        // methods
        //-----------------------------

        void setDefaultWindow(uiWindow *win);
        void setAppButtonText(const char *text);

        uint16_t indColor(uint8_t ind_state);
        void doText(const uiElement *ele, const char *text);
        void initProgress();
        void doTextProgress(const uiElement *ele, const char *text, bool changed);
        virtual void onButton(const uiElement *ele, bool pressed) override;

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

        // override to check for clicking on status bar

        bool hitTest();

};

extern gApplication the_app;
