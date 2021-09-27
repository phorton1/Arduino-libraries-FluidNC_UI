//--------------------------------------
// A dialog set zero positions
//--------------------------------------

#include "dlgSetPosition.h"
#include "gApp.h"

#include <gActions.h>   // FluidNC_extensions

#ifdef UI_WITH_MESH
    #include <Mesh.h>   // FluidNC_extensions
#endif

dlgSetPosition dlg_position;

//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------

#define IDX_CLEAR       6

#define ID_SET_ALL         (0x0001 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_SET_X           (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_SET_Y           (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_SET_Z           (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON)


static const uiElement set_position_elements[] =
{
    { ID_SET_ALL,      30,  45, 110,  33,  V("Zero All"), COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_SET_X,        30,  83, 110,  33,  V("X Zero"),   COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_SET_Y,        30, 121, 110,  33,  V("Y Zero"),   COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_SET_Z,        30, 159, 110,  33,  V("Z Zero"),   COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
};


//-----------------------
// implementation
//-----------------------

dlgSetPosition::dlgSetPosition() :
    uiWindow(set_position_elements,sizeof(set_position_elements)/sizeof(uiElement))
{
}


void dlgSetPosition::begin()
{
    uiWindow::begin();
    the_app.setTitle("Set Position");
}

void dlgSetPosition::update()
{
    uiWindow::update();
}



void dlgSetPosition::onButton(const uiElement *ele, bool pressed)
{
    if (!pressed)       // = released
    {
        switch (ele->id_type)
        {
            case ID_SET_ALL :
                gActions::pushGrblText("G10 L20 x0 y0 z0\r");
                break;
            case ID_SET_X :
                gActions::pushGrblText("G10 L20 x0\r");
                break;
            case ID_SET_Y :
                gActions::pushGrblText("G10 L20 y0\r");
                break;
            case ID_SET_Z :
                gActions::pushGrblText("G10 L20 z0\r");
                break;
        }
    }
}
