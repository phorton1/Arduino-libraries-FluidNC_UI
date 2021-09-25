//-----------------------------------------
// A dialog to pick differnt homing modes
//-----------------------------------------

#pragma once

#include "gApp.h"

class dlgHome : public uiWindow
{
    public:

        dlgHome();

    private:

        bool m_mesh_valid;

        virtual void begin() override;
        virtual void update() override;
        virtual bool isModal() const override { return true; }
        virtual void onButton(const uiElement *ele, bool pressed) override;

};


extern dlgHome dlg_home;
