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


            virtual void update() override {}


    };

#endif  // WITH_APPLICATION
