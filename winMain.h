//--------------------------------------
// The 'main' idle/nav window
//--------------------------------------

#pragma once

#include "gApp.h"

#define MAIN_MODE_IDLE    0
#define MAIN_MODE_ACTIVE  1


class winMain : public uiWindow
{
    public:

        winMain();

    private:

        int  m_mode;
        bool m_draw_needed;
        bool m_micro_mode;
        bool m_last_micro_mode;

        JobState m_last_job_state;
        indicatorState_t m_last_wifi_state;

        int     m_last_mode;
        uint8_t m_last_alarm;

        int m_last_feed_override;
        int m_last_spindle_override;

        #ifdef UI_WITH_MESH
            float m_last_live_z;
        #endif

        virtual void begin() override;
        virtual void update() override;
        virtual void onButton(const uiElement *ele, bool pressed) override;
        virtual const char *getMenuLabel() const override  { return "MAIN"; }

        void doJog(const char *axis, int jog_num);
        void setLastOverrideText();

};

extern winMain main_win;
