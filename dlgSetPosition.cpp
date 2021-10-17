//--------------------------------------
// A dialog set zero positions
//--------------------------------------

#include "dlgSetPosition.h"
#include "gApp.h"

#include <gActions.h>   // FluidNC_extensions

#ifdef UI_WITH_MESH
    #include <Mesh.h>   // FluidNC_extensions
    #include "dlgMeshSettings.h"
#endif

dlgSetPosition dlg_position;

//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------

#define ID_SET_ALL         (0x0001 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_SET_X           (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_SET_Y           (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_SET_Z           (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_SET_MESH        (0x0005 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_MESH_CLEAR      (0x0005 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_MESH_SHOW       (0x0007 | ID_TYPE_TEXT | ID_TYPE_BUTTON)

static uiMutable clear_mesh = {"Clear",  COLOR_DARKGREEN, COLOR_WHITE, FONT_NORMAL };

static const uiElement set_position_elements[] =
{
    { ID_SET_ALL,      30,  45, 110,  33,  V("Zero All"), COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_SET_X,        30,  83, 110,  33,  V("X Zero"),   COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_SET_Y,        30, 121, 110,  33,  V("Y Zero"),   COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_SET_Z,        30, 159, 110,  33,  V("Z Zero"),   COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
#ifdef UI_WITH_MESH
    { ID_SET_MESH,    180,  45, 110,  33,  V("Mesh"),     COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_MESH_CLEAR,  180, 121, 110,  33,  V(&clear_mesh) },
    { ID_MESH_SHOW,   180, 159, 110,  33,  V("Show"),     COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
#endif

};

#define IDX_CLEAR  6


//-----------------------
// implementation
//-----------------------

dlgSetPosition::dlgSetPosition() :
    uiWindow(set_position_elements,sizeof(set_position_elements)/sizeof(uiElement))
{
}


void dlgSetPosition::begin()
{
    the_app.setTitle("Set Position");
    #ifdef UI_WITH_MESH
        m_mesh_valid = !the_mesh.isValid();
    #endif
    uiWindow::begin();
}

void dlgSetPosition::update()
{
    #ifdef UI_WITH_MESH
        if (m_mesh_valid != the_mesh.isValid())
        {
            m_mesh_valid = the_mesh.isValid();
            clear_mesh.bg = m_mesh_valid ? COLOR_DARKGREEN : COLOR_BUTTON_DISABLED;
            clear_mesh.fg = m_mesh_valid ? COLOR_WHITE     : COLOR_LIGHTGREY;
            drawTypedElement(&set_position_elements[IDX_CLEAR]);
        }
    #endif
    uiWindow::update();
}



void dlgSetPosition::onButton(const uiElement *ele, bool pressed)
{
    if (!pressed)       // = released
    {
        switch (ele->id_type)
        {
            case ID_SET_ALL :
                gActions::pushGrblText("G10 L20 x0 y0 z0\r\n");
                break;
            case ID_SET_X :
                gActions::pushGrblText("G10 L20 x0\r\n");
                break;
            case ID_SET_Y :
                gActions::pushGrblText("G10 L20 y0\r\n");
                break;
            case ID_SET_Z :
                gActions::pushGrblText("G10 L20 z0\r\n");
                break;
            #ifdef UI_WITH_MESH
                case ID_SET_MESH:
                    the_app.openWindow(&dlg_mesh_settings);
                    break;
                case ID_MESH_CLEAR:
                    gActions::pushGrblText("$mesh/clear\r\n");
                    break;
                case ID_MESH_SHOW:
                    // the_app.openWindow(&dlg_mesh_settings);
                    break;
            #endif
        }
    }
}
