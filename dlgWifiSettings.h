//-----------------------------------------
// A dialog for WifiSettings
//-----------------------------------------

#pragma once

#include "gApp.h"


class dlgWifiSettings : public uiWindow
{
    public:

        dlgWifiSettings();

    private:

        bool draw_needed;

        virtual void begin() override;
        virtual bool isModal() const override { return true; }

};


extern dlgWifiSettings dlg_wifi_settings;
