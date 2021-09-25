//-----------------------------------------
// A dialog to pick differnt homing modes
//-----------------------------------------

#pragma once

#include "gApp.h"

class dlgSetPosition : public uiWindow
{
    public:

        dlgSetPosition();

    private:

        virtual void begin() override;
        virtual void update() override;
        virtual bool isModal() const override { return true; }
        virtual void onButton(const uiElement *ele, bool pressed) override;

};


extern dlgSetPosition dlg_position;
