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

        bool m_paused = false;

        virtual void update() override;

        virtual void onButton(const uiElement *ele, bool pressed) override;
};
