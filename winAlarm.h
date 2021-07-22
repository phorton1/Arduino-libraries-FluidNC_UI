//--------------------------------------
// The main Alaram window
//--------------------------------------

#pragma once

#include "gApp.h"

#ifdef WITH_APPLICATION

    class winAlarm : public uiWindow
    {
        public:

            winAlarm();

        private:

            virtual void update() override;
            void onButton(const uiElement *ele, bool pressed) override;
            virtual const char *getMenuLabel() const override  { return "ALARM"; }

    };

    extern winAlarm alarm_win;

#endif  // WITH_APPLICATION
