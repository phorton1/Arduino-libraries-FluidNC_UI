//--------------------------------------
// The main Alarm window
//--------------------------------------

#include "winAlarm.h"

#ifdef WITH_APPLICATION

    winAlarm alarm_win;

    //----------------------------------------------------------------------
    // WINDOW DEFINITION
    //----------------------------------------------------------------------

    #define ID_CLEAR_BUTTON  100
    #define ID_RESET_BUTTON  101

    static const uiElement alarm_elements[] = {

        { ID_CLEAR_BUTTON,  0,   0, 320, 240,   0,          COLOR_BLACK,    COLOR_WHITE, },
        { ID_RESET_BUTTON,         0,   0, 320,  35,   0,   COLOR_DARKBLUE, COLOR_WHITE, },
    };


    //----------------------
    // implementation
    //----------------------

    winAlarm::winAlarm() :
        uiWindow(alarm_elements,(sizeof(alarm_elements)/sizeof(uiElement)))
    {}

#endif  // WITH_APPLICATION
