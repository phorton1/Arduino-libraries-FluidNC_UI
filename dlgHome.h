//-----------------------------------------
// A dialog to pick differnt homing modes
//-----------------------------------------

#pragma once

#include "gApp.h"

class dlgHome : public uiWindow
{
    public:

        dlgHome();

        bool m_doing_probe;
            // set to true on a probe,
            // set to false by the_app, which
            // sets the z_position (for us, but
            // not for a general G38.2 probe)

    private:

        bool m_mesh_valid;

        virtual void begin() override;
        virtual void update() override;
        virtual bool isModal() const override { return true; }
        virtual void onButton(const uiElement *ele, bool pressed) override;

};


extern dlgHome dlg_home;
