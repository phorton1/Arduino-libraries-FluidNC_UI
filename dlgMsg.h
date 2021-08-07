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
        void setTimeout(uint32_t timeout)  {m_timeout = timeout;}
            // must be called AFTER setMsg

    private:

        virtual void update() override;
        virtual void onButton(const uiElement *ele, bool pressed) override;

        uint32_t m_timeout;


};


extern dlgMsg msg_dlg;

extern void okMsg(const char *line1, const char *line2="");
extern void errorMsg(const char *msg);
extern void warningMsg(const char *msg);
extern void doToast(uint16_t color, const char *line1, const char *line2="", uint32_t timeout=5000);
