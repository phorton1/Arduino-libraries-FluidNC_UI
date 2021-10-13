//--------------------------------------
// A dialog to reset or reboot the machine
//--------------------------------------

#include "dlgSystem.h"
#include "gApp.h"
#include "dlgConfirm.h"


#define ID_RETURN_BUTTON    (102 | ID_TYPE_TEXT | ID_TYPE_BUTTON )
#define ID_RESET_BUTTON     (103 | ID_TYPE_TEXT | ID_TYPE_BUTTON )
#define ID_REBOOT_BUTTON    (104 | ID_TYPE_TEXT | ID_TYPE_BUTTON )

static const uiElement system_elements[] =
{
    { ID_RETURN_BUTTON,   80,  50, 130,  40,  V("return"),   COLOR_BLUE,    COLOR_WHITE,    FONT_BIG   },
    { ID_RESET_BUTTON,    20, 145, 130,  40,  V("RESET"),    COLOR_ORANGE,  COLOR_BLACK,    FONT_BIG },
    { ID_REBOOT_BUTTON,  170, 145, 130,  40,  V("REBOOT"),   COLOR_DARKRED, COLOR_WHITE,    FONT_BIG },
};


dlgSystem dlg_system;

//-----------------------
// implementation
//-----------------------

dlgSystem::dlgSystem() :
    uiWindow(system_elements,sizeof(system_elements)/sizeof(uiElement))
{
}


void dlgSystem::begin()
{
    the_app.setTitle("SYSTEM");
    uiWindow::begin();
}



void dlgSystem::onButton(const uiElement *ele, bool pressed)
{
    if (!pressed)       // = released
    {
        switch (ele->id_type)
        {
            case ID_RETURN_BUTTON :
                the_app.endModal();
                break;
            case ID_RESET_BUTTON :
                the_app.endModal();
                dlg_confirm.setConfirm(CONFIRM_COMMAND_RESET);
                the_app.openWindow(&dlg_confirm);
                break;
            case ID_REBOOT_BUTTON :
                dlg_confirm.setConfirm(CONFIRM_COMMAND_REBOOT);
                the_app.openWindow(&dlg_confirm);
                break;
        }
    }
}
