//--------------------------------------
// The directory listing window
//--------------------------------------

#pragma once

#include "gApp.h"


#define MAX_FILENAME      128

class winFiles : public uiWindow
{
    public:

        winFiles();

        const char *getPath();
        const char *getFileToRun(int i);

    private:

        virtual void begin() override;
        virtual void update() override;
        virtual void onButton(const uiElement *ele, bool pressed) override;
        virtual const char *getMenuLabel() const override  { return "FILES"; }

};

extern winFiles files_win;
