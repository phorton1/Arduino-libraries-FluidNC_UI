//--------------------------------------
// A dialog to pick differnt homing modes
//--------------------------------------

#include "dlgHome.h"
#include "gApp.h"
#include <gActions.h>           // FluidNC_extensions

#ifdef UI_WITH_MESH
    #include <Mesh.h>           // FluidNC_extensions
#endif

dlgHome dlg_home;

//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------

#define IDX_CLEAR       6

#define ID_HOMEALL         (0x0001 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_HOMEX           (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_HOMEY           (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_HOMEZ           (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_PROBE           (0x0005 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_MESH_CREATE     (0x0006 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_MESH_CLEAR      (0x0007 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_MESH_SHOW        (0x0008 | ID_TYPE_TEXT | ID_TYPE_BUTTON)

static uiMutable clear_mesh = {"Clear",  COLOR_DARKGREEN, COLOR_WHITE, FONT_NORMAL };

static const uiElement home_elements[] =
{
    { ID_HOMEALL,      30,  45, 110,  33,  V("Home All"), COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_HOMEX,        30,  83, 110,  33,  V("Home X"),   COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_HOMEY,        30, 121, 110,  33,  V("Home Y"),   COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_HOMEZ,        30, 159, 110,  33,  V("Home Z"),   COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_PROBE,       180,  45, 110,  33,  V("Probe"),    COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
#ifdef UI_WITH_MESH
    { ID_MESH_CREATE, 180,  83, 110,  33,  V("Mesh"),     COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
    { ID_MESH_CLEAR,  180, 121, 110,  33,  V(&clear_mesh) },
    { ID_MESH_SHOW,   180, 159, 110,  33,  V("Show"), COLOR_DARKGREEN, COLOR_WHITE,  FONT_NORMAL, JUST_CENTER},
#endif
};




//-----------------------
// implementation
//-----------------------

dlgHome::dlgHome() :
    uiWindow(home_elements,sizeof(home_elements)/sizeof(uiElement))
{
    m_mesh_valid = 0;
    m_doing_probe = 0;
}


void dlgHome::begin()
{
    #ifdef UI_WITH_MESH
        m_mesh_valid = !the_mesh.isValid();
    #endif
    the_app.setTitle(
        #ifdef UI_WITH_MESH
            "Home / Mesh"
        #else
            "Home"
        #endif
        );
    uiWindow::begin();
}


void dlgHome::update()
{
    #ifdef UI_WITH_MESH
        if (m_mesh_valid != the_mesh.isValid())
        {
            m_mesh_valid = the_mesh.isValid();
            clear_mesh.bg = m_mesh_valid ? COLOR_DARKGREEN : COLOR_BUTTON_DISABLED;
            clear_mesh.fg = m_mesh_valid ? COLOR_WHITE     : COLOR_LIGHTGREY;
            drawTypedElement(&home_elements[IDX_CLEAR]);
        }
    #endif
}



void dlgHome::onButton(const uiElement *ele, bool pressed)
{
    if (!pressed)       // = released
    {
        switch (ele->id_type)
        {
            case ID_HOMEALL :
                gActions::pushGrblText("$H\r\n");
                break;
            case ID_HOMEX :
                gActions::pushGrblText("$HX\r\n");
                break;
            case ID_HOMEY :
                gActions::pushGrblText("$HY\r\n");
                break;
            case ID_HOMEZ :
                gActions::pushGrblText("$HZ\r\n");
                break;

            case ID_PROBE :
                // clear the global FluidNC probe success flag JIC
                // start the probe, and set a flag that we started a probe,
                // it will be finished in winMain::update()
                g_debug("PROBE STARTED");
                gActions::clearProbeSucceeded();
                gActions::pushGrblText("G38.2 F100 Z-50\r\n");
                m_doing_probe = true;
                break;

            #ifdef UI_WITH_MESH
                case ID_MESH_CREATE:
                    gActions::pushGrblText("$mesh/do_level\r\n");
                    break;
                case ID_MESH_CLEAR:
                    gActions::pushGrblText("$mesh/clear\r\n");
                    return;     // stay in window
                    break;
                case ID_MESH_SHOW:
                    // the_app.openWindow(&dlg_mesh_settings);
                    return;     // stay in window
                    break;
            #endif
        }

        // end the window (return to main window) for most cases

        the_app.endModal();
    }
}
