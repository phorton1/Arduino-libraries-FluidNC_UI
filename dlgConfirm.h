//--------------------------------------
// The confirm Yes/No window
//--------------------------------------

#pragma once

#include "gApp.h"

#ifdef WITH_APPLICATION

    #define CONFIRM_COMMAND_RESET 0x0001
    #define CONFIRM_COMMAND_REBOOT  0x0002

    class dlgConfirm : public uiWindow
    {
        public:

            dlgConfirm();

            virtual bool isModal() const override { return true; }

            void setConfirm(uint16_t command);

        private:

            virtual void onButton(const uiElement *ele, bool pressed) override;

    };


    extern dlgConfirm confirm_dlg;

#endif  // WITH_APPLICATION
