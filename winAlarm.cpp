//--------------------------------------
// The main Alarm window
//--------------------------------------

#include "winAlarm.h"

#ifdef WITH_APPLICATION

    #define DEBUG_ALARM  0

    #include "dlgConfirm.h"

    #ifdef WITH_GRBL
        #include <System.h>
        #include <WebUI/InputBuffer.h>
    #endif

    winAlarm alarm_win;
    static bool g_draw_needed = false;
    static uint8_t g_last_alarm = 255;

    const char *alarmText(uint8_t alarm)
    {
        switch (alarm)
        {
            case 0  : return "Startup Alarm !!";
            case 1  : return "Hard Limit Alarm !!";
            case 2  : return "Soft Limit Alarm !!";
            case 3  : return "Abort Cycle Alarm !!";
            case 4  : return "Probe Fail Initial !!";
            case 5  : return "Probe Fail Contact !!";
            case 6  : return "Homing Fail Reset !!";
            case 7  : return "Homing Fail Door !!";
            case 8  : return "Homing Fail Pulloff !!";
            case 9  : return "Homing Fail Approach !!";
            case 10 : return "Spindle Control !!";
        }
        return "UNKNOWN_ALARM";
    }

    //----------------------------------------------------------------------
    // WINDOW DEFINITION
    //----------------------------------------------------------------------

    #define IDX_ALARM_TEXT      0
    #define IDX_HOME_BUTTON     1
    #define IDX_CLEAR_BUTTON    2

    #define ID_ALARM_TEXT       (0x0001 )
    #define ID_HOME_BUTTON      (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
    #define ID_CLEAR_BUTTON     (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
    #define ID_RESET_BUTTON     (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON )
    #define ID_REBOOT_BUTTON    (0x0005 | ID_TYPE_TEXT | ID_TYPE_BUTTON )


    static uiMutable home_button = {
        "HOME",
        COLOR_BLUE,
        COLOR_WHITE,
        FONT_BIG,
    };
    static uiMutable clear_button = {
        "CLEAR",
        COLOR_BUTTON_DISABLED,
        COLOR_WHITE,
        FONT_BIG,
    };

    static const uiElement alarm_elements[] =
    {
        { ID_ALARM_TEXT,      0,   45, 320,  30,    0,               COLOR_BLACK,   COLOR_MAGENTA,   FONT_BIG },
        { ID_HOME_BUTTON,     20,  95, 130,  40,    V(&home_button)  },
        { ID_CLEAR_BUTTON,   170,  95, 130,  40,    V(&clear_button) },
        { ID_RESET_BUTTON,    20, 145, 130,  40,    V("RESET"),      COLOR_ORANGE,  COLOR_BLACK,     FONT_BIG },
        { ID_REBOOT_BUTTON,  170, 145, 130,  40,    V("REBOOT"),     COLOR_DARKRED, COLOR_WHITE,     FONT_BIG },
    };



    //----------------------
    // implementation
    //----------------------

    winAlarm::winAlarm() :
        uiWindow(alarm_elements,(sizeof(alarm_elements)/sizeof(uiElement)))
    {}

    void winAlarm::begin()
    {
        uiWindow::begin();
        g_draw_needed = true;
    }

    void winAlarm::onButton(const uiElement *ele, bool pressed)
        // called before drawElement
    {
        if (!pressed)
        {
            switch (ele->id_type)
            {
                case ID_HOME_BUTTON :
                    the_app.setTitle("");
                    #ifdef WITH_GRBL
                        WebUI::inputBuffer.push("$H\r");
                    #endif
                    break;
                case ID_CLEAR_BUTTON :
                    the_app.setTitle("");
                    #ifdef WITH_GRBL
                        WebUI::inputBuffer.push("$X\r");
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


    void winAlarm::update()
    {
        // disable home and clear button if busy or homing
        // they report the alarm, then reset the machine which clears the alarm number, but not the state
        // so we only grab the alarm number if the state has changed

        volatile uint8_t alarm = g_last_alarm;
        JobState jstate = the_app.getJobState();

        if (jstate != the_app.getLastJobState())
        {
            #ifdef WITH_GRBL
                alarm = static_cast<uint8_t>(sys_rt_exec_alarm);
                #if DEBUG_ALARM
                    g_debug("grabbed alarm=%d",alarm);
                #endif
            #endif
        }

        if (g_draw_needed ||
            g_last_alarm != alarm ||
            jstate != the_app.getLastJobState())
        {
            g_draw_needed = false;
            g_last_alarm = alarm;

            bool is_alarm = jstate == JOB_ALARM;
            bool busy =
                jstate == JOB_BUSY ||
                jstate == JOB_HOLD ||
                jstate == JOB_HOMING;

            #if DEBUG_ALARM
                g_debug("doing alarm buttons is_alarm(%d) busy(%d)",is_alarm,busy);
            #endif

            clear_button.bg = is_alarm ? COLOR_BLUE : COLOR_BUTTON_DISABLED;
            drawTypedElement(&m_elements[IDX_CLEAR_BUTTON]);
            home_button.bg = busy ? COLOR_BUTTON_DISABLED : COLOR_BLUE;
            drawTypedElement(&m_elements[IDX_HOME_BUTTON]);

            char buf[80] = {0};
            if (is_alarm)
                sprintf(buf,"ALARM %d",alarm);
            the_app.setTitle(buf);

            buf[0] = 0;
            if (is_alarm)
            {
                strcpy(buf,alarmText(alarm));
                #if DEBUG_ALARM
                    g_debug("winAlarm::alarm %d : %s",alarm,buf);
                #endif
            }

            const uiElement *ele = &m_elements[IDX_ALARM_TEXT];
            drawText(
                buf,
                ele->just,
                ele->font,
                ele->x, ele->y, ele->w, ele->h,
                ele->fg,
                ele->bg );
        }
    }


#endif  // WITH_APPLICATION
