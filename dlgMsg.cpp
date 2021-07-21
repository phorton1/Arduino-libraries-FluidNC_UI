//--------------------------------------
// The msg, error, and warning windows
//--------------------------------------

#include "dlgMsg.h"

#ifdef WITH_APPLICATION

    dlgMsg msg_dlg;

    //----------------------------------------------------------------------
    // WINDOW DEFINITION
    //----------------------------------------------------------------------

    #define ID_MSG1        (0x0001 | ID_TYPE_TEXT | ID_TYPE_MUTABLE )
    #define ID_MSG2        (0x0002 | ID_TYPE_TEXT | ID_TYPE_MUTABLE )
    #define ID_OK_BUTTON   (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON  )

    static uiMutable msg1 = {
        "",
        COLOR_BLACK,
        COLOR_WHITE,
        FONT_BIG,
    };

    static uiMutable msg2 = {
        "",
        COLOR_BLACK,
        COLOR_WHITE,
        FONT_BIG,
    };


    const uiElement msg_elements[] =
    {
        { ID_MSG1,         0,  60, 320,  25,   V(&msg1), },
        { ID_MSG2,         0,  85, 320,  25,   V(&msg2), },
        { ID_OK_BUTTON,  110, 130, 100,  50,   V("OK"),  COLOR_BLUE,  COLOR_WHITE, FONT_BIG,},
    };


    //------------------
    // implementation
    //------------------

    dlgMsg::dlgMsg() :
        uiWindow(msg_elements,(sizeof(msg_elements)/sizeof(uiElement)))
    {}

    void dlgMsg::setMsg(uint16_t color, const char *line1, const char *line2 /*=""*/)
    {
        msg1.fg = color;
        msg1.text = line1;
        msg2.fg = color;
        msg2.text = line2;
    }


    void dlgMsg::onButton(const uiElement *ele, bool pressed)
        // called before drawElement
    {
        if (!pressed)
        {
            the_app.endModal();
        }
    }


    void okMsg(const char *line1, const char *line2 /*=""*/)
    {
        msg_dlg.setMsg(COLOR_WHITE,line1,line2);
        the_app.openWindow(&msg_dlg);
    }

    void errorMsg(const char *msg)
    {
        msg_dlg.setMsg(COLOR_RED,"ERROR",msg);
        the_app.openWindow(&msg_dlg);
    }

    void warningMsg(const char *msg)
    {
        msg_dlg.setMsg(COLOR_YELLOW,"WARNING",msg);
        the_app.openWindow(&msg_dlg);
    }

#endif  // WITH_APPLICATION