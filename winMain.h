//--------------------------------------
// The 'main' idle/nav window
//--------------------------------------

#pragma once

#include "gApp.h"

#ifdef WITH_APPLICATION

    class winMain : public uiWindow
    {
        public:

            winMain();

        private:

            virtual void onButton(const uiElement *ele, bool pressed) override;
            virtual const char *getMenuLabel() const override  { return "MAIN"; }

    };

    extern winMain main_win;

#endif  // WITH_APPLICATION