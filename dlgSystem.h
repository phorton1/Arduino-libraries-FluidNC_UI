//-----------------------------------------
// A dialog to pick differnt homing modes
//-----------------------------------------

#pragma once

#include "gApp.h"

class dlgSystem : public uiWindow
{
    public:

        dlgSystem();

    private:

        virtual void begin() override;
        virtual bool isModal() const override { return true; }
        virtual void onButton(const uiElement *ele, bool pressed) override;

};


extern dlgSystem dlg_system;
