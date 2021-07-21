//--------------------------------------
// The main Alarm window
//--------------------------------------

#include "dlgConfirm.h"

#ifdef WITH_APPLICATION


    #define ID_LINE1        (0x0001 | ID_TYPE_TEXT)
    #define ID_LINE2        (0x0002 | ID_TYPE_TEXT | ID_TYPE_MUTABLE )
    #define ID_YES_BUTTON   (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON  )
    #define ID_NO_BUTTON    (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON  )

    #define RU_SURE     "Are you sure you want to"

    //----------------------------------------------------------------------
    // WINDOW DEFINITION
    //----------------------------------------------------------------------

    uiMutable confirm_msg = {
        "",
        COLOR_BLACK,
        COLOR_WHITE,
        FONT_BIG,
    };

    const uiElement confirm_elements[] =
    {
        { ID_LINE1,        0,  60, 320,  20,   V(RU_SURE),      COLOR_BLACK, COLOR_WHITE, FONT_BIG,},
        { ID_LINE2,        0,  80, 320,  20,   V(&confirm_msg), },
        { ID_YES_BUTTON,  40, 130, 100,  50,   V("YES"),        COLOR_BLUE,  COLOR_WHITE, FONT_BIG,},
        { ID_NO_BUTTON,  180, 130, 100,  50,   V("NO"),         COLOR_BLUE,  COLOR_WHITE, FONT_BIG,},
    };




    dlgConfirm::dlgConfirm() :
        uiWindow(confirm_elements,(sizeof(confirm_elements)/sizeof(uiElement)))
    {}


    void dlgConfirm::setMessage(const char *msg)
    {
        confirm_msg.text = msg;
    }


    void dlgConfirm::onButton(const uiElement *ele, bool pressed)
        // called before drawElement
    {
        if (!pressed)
        {
            the_app.endConfirm(ele->id_type == ID_YES_BUTTON);
        }
    }


#endif  // WITH_APPLICATION
