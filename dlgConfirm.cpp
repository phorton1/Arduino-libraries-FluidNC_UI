//--------------------------------------
// The confirm Yes/No window
//--------------------------------------

#include "dlgConfirm.h"

#ifdef WITH_APPLICATION

    #define ID_LINE1        (0x0001 | ID_TYPE_TEXT)
    #define ID_LINE2        (0x0002 | ID_TYPE_TEXT | ID_TYPE_MUTABLE )
    #define ID_YES_BUTTON   (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON  )
    #define ID_NO_BUTTON    (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON  )

    #define RU_SURE     "Are you sure you want to"

    dlgConfirm  confirm_dlg;
    static uint16_t pending_command;

    //----------------------------------------------------------------------
    // WINDOW DEFINITION
    //----------------------------------------------------------------------

    static uiMutable confirm_msg = {
        "",
        COLOR_BLACK,
        COLOR_WHITE,
        FONT_BIG,
    };

    static const uiElement confirm_elements[] =
    {
        { ID_LINE1,        0,  60, 320,  25,   V(RU_SURE),      COLOR_BLACK, COLOR_WHITE, FONT_BIG,},
        { ID_LINE2,        0,  85, 320,  25,   V(&confirm_msg), },
        { ID_YES_BUTTON,  40, 130, 100,  50,   V("YES"),        COLOR_BLUE,  COLOR_WHITE, FONT_BIG,},
        { ID_NO_BUTTON,  180, 130, 100,  50,   V("NO"),         COLOR_BLUE,  COLOR_WHITE, FONT_BIG,},
    };


    //-----------------------
    // implementation
    //-----------------------

    dlgConfirm::dlgConfirm() :
        uiWindow(confirm_elements,(sizeof(confirm_elements)/sizeof(uiElement)))
    {}


    void dlgConfirm::setConfirm(uint16_t command)
    {
        confirm_msg.text =
             command == CONFIRM_COMMAND_RESET ? "reset the GRBL Machine?" :
             command == CONFIRM_COMMAND_REBOOT ? "reboot the Controller?" :
             "UNKNOWN CONFIRM COMMAND";
         pending_command = command;
    }


    void dlgConfirm::onButton(const uiElement *ele, bool pressed)
        // called before drawElement
    {
        if (!pressed)
        {
            if (ele->id_type == ID_YES_BUTTON)
            {
                if (pending_command == CONFIRM_COMMAND_RESET)
                {
                     #ifdef WITH_GRBL
                        execute_realtime_command(Cmd::Reset,CLIENT_ALL);
                    #endif
                }
                else if (pending_command == CONFIRM_COMMAND_REBOOT)
                {
                    debug_serial("gApplication estarting the ESP32!!");
                    delay(500);
                    ESP.restart();
                    while (1) {}
                }
            }
            the_app.endModal();
        }
    }


#endif  // WITH_APPLICATION
