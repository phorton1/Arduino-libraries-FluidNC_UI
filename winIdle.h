//--------------------------------------
// The 'busy' window
//--------------------------------------

#pragma once

#include "gApp.h"

#ifdef WITH_APPLICATION

    class winIdle : public uiWindow
    {
        public:

            winIdle();

        private:

            virtual void onButton(const uiElement *ele, bool pressed) override;

    };


#endif  // WITH_APPLICATION