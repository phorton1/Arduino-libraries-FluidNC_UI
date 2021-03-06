//--------------------------------------
// The confirm Yes/No window
//--------------------------------------

#pragma once

#include "gApp.h"

#define CONFIRM_COMMAND_RESET       0x0001
#define CONFIRM_COMMAND_REBOOT      0x0002
#define CONFIRM_COMMAND_SET_CONFIG  0x0010
#define CONFIRM_COMMAND_RUN_FILE    0x0100



class dlgConfirm : public uiWindow
{
    public:

        dlgConfirm();

        virtual bool isModal() const override { return true; }

        void setConfirm(uint16_t command);

    private:

        virtual void onButton(const uiElement *ele, bool pressed) override;

};


extern dlgConfirm dlg_confirm;
