//--------------------------------------
// The 'busy' window
//--------------------------------------

#pragma once

#include "gApp.h"


class winIdle : public uiWindow
{
    public:

        winIdle();

    private:

        virtual void onButton(const uiElement *ele, bool pressed) override;

};
