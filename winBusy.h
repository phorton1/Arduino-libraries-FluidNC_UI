//--------------------------------------
// The main Busy window
//--------------------------------------

#pragma once

#include "gApp.h"

class winBusy : public uiWindow
{
    public:

        winBusy();

    private:

        JobState m_job_state;

        void setElements();

        virtual void begin() override;
        virtual void update() override;
        virtual void onButton(const uiElement *ele, bool pressed) override;
        virtual const char *getMenuLabel() const override  { return "BUSY"; }

};

extern winBusy busy_win;
