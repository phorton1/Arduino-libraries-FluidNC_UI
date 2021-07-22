//--------------------------------------
// The directory listing window
//--------------------------------------

#pragma once

#include "gApp.h"

#ifdef WITH_APPLICATION

    class winFiles : public uiWindow
    {
        public:

            winFiles();

            const char *getFileToRun(int i);
            const char *getFileQuestion(int i);


        private:

            virtual void begin() override;
            virtual void update() override;
            virtual void onButton(const uiElement *ele, bool pressed) override;
            virtual const char *getMenuLabel() const override  { return "FILES"; }

    };

    extern winFiles files_win;

#endif  // WITH_APPLICATION
