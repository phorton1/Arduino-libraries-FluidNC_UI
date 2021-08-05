//--------------------------------------
// The main Busy window
//--------------------------------------

#include "winBusy.h"
#include "dlgConfirm.h"

#ifdef WITH_GRBL
    #include <Serial.h>     // for CLIENTS and execute_realtime_command() and Cmd types
        // have to modify Serial.h to directly include "Config.h" to get CLIENT_TOUCH_UI
#endif


winBusy busy_win;


//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------

#define IDX_RESUME_BUTTON    0      // index to ID_PAUSE_RESUME_BUTTON in element array

#define ID_PAUSE_RESUME_BUTTON      (0x0001 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_RESET_BUTTON             (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON )
#define ID_REBOOT_BUTTON            (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON )

static uiMutable pr_button = {
    "PAUSE",
    COLOR_BLUE,
    COLOR_WHITE,
    FONT_BIG,
};

static const uiElement busy_elements[] =
{
    { ID_PAUSE_RESUME_BUTTON,  20,  70, 130,  90,    V(&pr_button),  },
    { ID_RESET_BUTTON,        170,  60, 130,  40,    V("RESET"),     COLOR_ORANGE,  COLOR_BLACK, FONT_BIG },
    { ID_REBOOT_BUTTON,       170, 130, 130,  40,    V("REBOOT"),    COLOR_DARKRED, COLOR_WHITE, FONT_BIG },
};


//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------


winBusy::winBusy() :
    uiWindow(busy_elements,(sizeof(busy_elements)/sizeof(uiElement)))
{}

void winBusy::setElements()
{
    pr_button.text = m_job_state == JOB_HOLD ? "RESUME" : "PAUSE";
    pr_button.bg   =
        m_job_state == JOB_HOLD ? COLOR_DARKGREEN :
        m_job_state == JOB_BUSY ? COLOR_BLUE :
        COLOR_BUTTON_DISABLED;
}

void winBusy::begin()
{
    m_job_state = the_app.getJobState();
    setElements();
    uiWindow::begin();
}


void winBusy::onButton(const uiElement *ele, bool pressed)
    // called before drawElement
{
    if (!pressed)
    {
        switch (ele->id_type)
        {
            case ID_PAUSE_RESUME_BUTTON :
                #ifdef WITH_GRBL
                    execute_realtime_command(
                        m_job_state == JOB_HOLD ?
                            Cmd::CycleStart : Cmd::FeedHold,
                        CLIENT_ALL);
                #endif
                break;
            case ID_RESET_BUTTON :
                confirm_dlg.setConfirm(CONFIRM_COMMAND_RESET);
                the_app.openWindow(&confirm_dlg);
                break;
            case ID_REBOOT_BUTTON :
                confirm_dlg.setConfirm(CONFIRM_COMMAND_REBOOT);
                the_app.openWindow(&confirm_dlg);
                break;
        }
    }
}


void winBusy::update()
{
    JobState job_state = the_app.getJobState();
    if (m_job_state != job_state)
    {
        m_job_state = job_state;
        setElements();
        drawTypedElement(&m_elements[IDX_RESUME_BUTTON]);
    }
}
