//--------------------------------------
// The main Busy window
//--------------------------------------

#include "winBusy.h"
#include "Grbl_MinUI.h"    // for debug_serial() and TEST_STANDALONE_UI

#if !TEST_STANDALONE_UI
    #include <Serial.h>     // for CLIENTS and execute_realtime_command() and Cmd types
        // have to modify Serial.h to directly include "Config.h" to get CLIENT_TOUCH_UI
#endif

#if TEST_STANDALONE_UI
    blah
#endif


//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------


#define ID_PAUSE_RESUME_BUTTON      (0x0001 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_RESET_BUTTON             (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON )
#define ID_REBOOT_BUTTON            (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON )


uiMutable pr_button = {
    "PAUSE",
    COLOR_BLUE,
    COLOR_WHITE,
    FONT_BIG,
};


const uiElement busy_elements[] =
{
    { ID_PAUSE_RESUME_BUTTON, 20,  70, 130,  90,    V(&pr_button),  },
    { ID_RESET_BUTTON,       170,  60, 130,  40,    V("RESET"),         COLOR_ORANGE,  COLOR_BLACK, FONT_BIG },
    { ID_REBOOT_BUTTON,      170, 130, 130,  40,    V("REBOOT"),        COLOR_DARKRED, COLOR_WHITE, FONT_BIG },
};



//----------------------------------------------------------------------
// methods
//----------------------------------------------------------------------


winBusy::winBusy() :
    uiWindow(busy_elements,(sizeof(busy_elements)/sizeof(uiElement)))
{}




void winBusy::onButton(const uiElement *ele, bool pressed)
    // called before drawElement
{
    if (!pressed)
    {
        if (ele->id_type == ID_PAUSE_RESUME_BUTTON)
        {
            #if !TEST_STANDALONE_UI
                execute_realtime_command(
                    m_paused ? Cmd::CycleStart : Cmd::FeedHold,
                    CLIENT_ALL);
            #endif
        }
        else if (ele->id_type == ID_RESET_BUTTON)
        {
            #if !TEST_STANDALONE_UI
                execute_realtime_command(Cmd::Reset,CLIENT_ALL);
                    // CLIENT_TOUCH_UI);
            #endif
        }
        else if (ele->id_type == ID_REBOOT_BUTTON)
        {
            debug_serial("winBusy::onButton(ID_REBOOT_BUTTON) restarting the ESP32!!");
            delay(500);
            ESP.restart();
            while (1) {}
        }
    }
}


void winBusy::update()
{
    bool paused = g_status.getSysState() == grbl_State_t::Hold;
    if (m_paused != paused)
    {
        m_paused = paused;

        pr_button.text = paused ? "RESUME" : "PAUSE";
        pr_button.bg   = paused ? COLOR_DARKGREEN : COLOR_BLUE;
        drawTypedElement(&m_elements[0]);
            // the resume button just happens to be element 0 in this window
            // will probably added a drawTypedElement(uint16_t, bool pressed)
            // that takes an "id_type"
    }

}
