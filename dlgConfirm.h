//--------------------------------------
// The confirm Yes/No window
//--------------------------------------

#pragma once

#include "gApp.h"

#ifdef WITH_APPLICATION

    class dlgConfirm : public uiWindow
    {
        public:

            dlgConfirm();

            void setMessage(const char *msg);


        private:


            virtual void onButton(const uiElement *ele, bool pressed) override;

    };

#endif  // WITH_APPLICATION
