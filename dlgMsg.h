//--------------------------------------
// The msg, error, and warning windows
//--------------------------------------

#pragma once

#include "gApp.h"

class dlgMsg : public uiWindow
{
    public:

        dlgMsg();

        virtual bool isModal() const override { return true; }

        void setMsg(uint16_t color, const char *line1, const char *line2="");

    private:

        virtual void onButton(const uiElement *ele, bool pressed) override;

};


extern dlgMsg msg_dlg;

extern void okMsg(const char *line1, const char *line2="");
extern void errorMsg(const char *msg);
extern void warningMsg(const char *msg);
