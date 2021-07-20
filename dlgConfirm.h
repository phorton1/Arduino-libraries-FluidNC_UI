//--------------------------------------
// The confirm Yes/No window
//--------------------------------------

#pragma once

#include "gApp.h"


class dlgConfirm : public uiWindow
{
    public:

        dlgConfirm();

        void setMessage(const char *msg);


    private:


        virtual void onButton(const uiElement *ele, bool pressed) override;

};
