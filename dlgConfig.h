//-------------------------------------------------
// A dialog to select a config.yaml from the SPFFS
//-------------------------------------------------

#pragma once

#include "gApp.h"

class dlgConfig : public uiWindow
{
    public:

        dlgConfig();

        void setConfigFilename(int config_num);
        const char *getConfigFilename(int config_num);

    private:

        virtual void begin() override;
        virtual bool isModal() const override { return true; }
        virtual void onButton(const uiElement *ele, bool pressed) override;


};


extern dlgConfig dlg_config;
