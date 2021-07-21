//--------------------------------------
// The main menu is a dialog window
//--------------------------------------

#pragma once

#include "gApp.h"

#ifdef WITH_APPLICATION


    class dlgMainMenu : public uiWindow
    {
        public:

            dlgMainMenu();

            virtual void begin();
            virtual bool isModal() const override { return true; }

        private:

            virtual void onButton(const uiElement *ele, bool pressed) override;

    };


    extern dlgMainMenu main_menu;

#endif  // WITH_APPLICATION
