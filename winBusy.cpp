//--------------------------------------
// The main Busy window
//--------------------------------------

#include "winBusy.h"
#include "Grbl_MinUI.h"    // for debug_serial() and TEST_STANDALONE_UI

#if !TEST_STANDALONE_UI
    #include <Serial.h>     // for CLIENTS and execute_realtime_command() and Cmd types
        // have to modify Serial.h to directly include "Config.h" to get CLIENT_TOUCH_UI
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


#define IDX_RESUME_BUTTON    0      // index to ID_PAUSE_RESUME_BUTTON in element array


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
            // should NOT get this if the window is disabled
            // via COLOR_BUTTON_DISABLED

            #if !TEST_STANDALONE_UI
                execute_realtime_command(
                    m_paused ? Cmd::CycleStart : Cmd::FeedHold,
                    CLIENT_ALL);
            #endif
        }
        else if (ele->id_type == ID_RESET_BUTTON)
        {
            the_app.confirmCommand(CONFIRM_COMMAND_RESET);
        }
        else if (ele->id_type == ID_REBOOT_BUTTON)
        {
            the_app.confirmCommand(CONFIRM_COMMAND_REBOOT);
        }
    }
}


void winBusy::update()
{
    bool busy = g_status.getSDState() == grbl_SDState_t::Busy;
    bool paused = g_status.getSysState() == grbl_State_t::Hold;

    if (m_busy != busy ||
        m_paused != paused)
    {
        m_busy = busy;
        m_paused = paused;

        pr_button.text = paused ? "RESUME" : "PAUSE";
        pr_button.bg   =
            !busy ? COLOR_BUTTON_DISABLED :
            paused ? COLOR_DARKGREEN : COLOR_BLUE;
        drawTypedElement(&m_elements[IDX_RESUME_BUTTON]);
    }

}
