//--------------------------------------
// The Grbl_MinUI application class
//--------------------------------------

#pragma once

#include "uiWindow.h"

#ifdef WITH_APPLICATION

    #include "gStatus.h"

    typedef enum JobState
    {
        JOB_IDLE,
        JOB_BUSY,
        JOB_HOLD,
        JOB_ALARM
    };

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
        grbl_State_t   sys_state;
        grbl_SDState_t sd_state;
        float          pct;
        float          pos[UI_NUM_AXES * 2];

        // gApplication change detection

        int16_t        prog_w;
        uint16_t       prog_color;
        char           app_title[UI_MAX_TITLE + 1];
        char           app_button[UI_MAX_BUTTON + 1];

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

            JobState getJobState()  { return job_state; }

            const char *getAppButtonText();
            void setAppButtonText(const char *text);


        private:

            JobState job_state;

            bool draw_needed;

            // progress bar

            int16_t  prog_x;
            int16_t  prog_w;
            uint16_t prog_color;

            // previous state

            appLast_t last;

            //-----------------------------
            // methods
            //-----------------------------

            void setDefaultWindow();

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

    };


    extern gApplication the_app;

#endif  // WITH_APPLICATION
