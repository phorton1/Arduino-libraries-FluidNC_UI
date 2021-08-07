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
        uint8_t m_last_alarm;


        virtual void begin() override;
        virtual void update() override;
        virtual void onButton(const uiElement *ele, bool pressed) override;
        virtual const char *getMenuLabel() const override  { return "MAIN"; }

        void doJog(const char *axis, int jog_num);

};

extern winMain main_win;
