//-----------------------------------------
// A dialog to modify messh settings
//-----------------------------------------
// An item can be the "selected" item and can be edited
// An item that is selected and changed becomes magenta
// When an item is changed, the status bar is overritten by a big "SAVE" button
// An item can be saved, or the changes will be lost
// Saved items show as green
//
// There is a save button that is enabled when an item is changed
// The whole window can go away if something happens in the machine
// Return to previous window is akward (press "MAIN" to return to
// the dlgHome window)
//
// Items are truncated to integers for this starting editor.
// Some things can be change by 10
//
//   [plus 10]
//   [plus 1]
//   [minus 1]
//   [minus 10]
//
// STATUS BAR = SAVE

#include "dlgMeshSettings.h"

#ifdef UI_WITH_MESH
    // nothing compiled if !UI_WITH_MESH!!

#include <Mesh.h>       // FluidNC_extensions
#include <gActions.h>   // FluidNC_extensions


dlgMeshSettings dlg_mesh_settings;


//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------

#define BTN_IDX_SAVE         0
#define BTN_IDX_PLUS10       1
#define BTN_IDX_PLUS1        2
#define BTN_IDX_MINUS1       3
#define BTN_IDX_MINUS10      4

#define NUM_BUTTONS          5

#define ID_SAVE_BUTTON     (0x0001 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
#define ID_PLUS10_BUTTON   (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
#define ID_PLUS1_BUTTON    (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
#define ID_MINUS1_BUTTON   (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
#define ID_MINUS10_BUTTON  (0x0005 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )

#define FIRST_PARAM_NUMBER  0x0010

#define ID_WIDTH         ((FIRST_PARAM_NUMBER + 0) | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_NO_BORDER | ID_TYPE_DUAL)
#define ID_HEIGHT        ((FIRST_PARAM_NUMBER + 1) | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_NO_BORDER | ID_TYPE_DUAL)
#define ID_X_STEPS       ((FIRST_PARAM_NUMBER + 2) | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_NO_BORDER | ID_TYPE_DUAL)
#define ID_Y_STEPS       ((FIRST_PARAM_NUMBER + 3) | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_NO_BORDER | ID_TYPE_DUAL)
#define ID_NUM_PROBES    ((FIRST_PARAM_NUMBER + 4) | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_NO_BORDER | ID_TYPE_DUAL)

#define NUM_PARAMS   5


static int mesh_value[NUM_PARAMS];


static uiMutable buttons[NUM_BUTTONS] =
{
    { "SAVE", COLOR_BUTTON_DISABLED, COLOR_WHITE, FONT_NORMAL },
    { "+10",  COLOR_BUTTON_HIDDEN,   COLOR_WHITE, FONT_NORMAL },
    { "+1",   COLOR_BUTTON_HIDDEN,   COLOR_WHITE, FONT_NORMAL },
    { "-1",   COLOR_BUTTON_HIDDEN,   COLOR_WHITE, FONT_NORMAL },
    { "-10",  COLOR_BUTTON_HIDDEN,   COLOR_WHITE, FONT_NORMAL },
};

static uiDualElement params[NUM_PARAMS] =
{
    { ELEMENT_TYPE_INT, COLOR_BLUE, 190, 70, "Width",      &mesh_value[0], FONT_SMALL },
    { ELEMENT_TYPE_INT, COLOR_BLUE, 190, 70, "Height",     &mesh_value[1], FONT_SMALL },
    { ELEMENT_TYPE_INT, COLOR_BLUE, 190, 70, "X_Steps",    &mesh_value[2], FONT_SMALL },
    { ELEMENT_TYPE_INT, COLOR_BLUE, 190, 70, "Y_Steps",    &mesh_value[3], FONT_SMALL },
    { ELEMENT_TYPE_INT, COLOR_BLUE, 190, 70, "Num_Probes", &mesh_value[4], FONT_SMALL },
};


#define IDX_PARAMS  5
#define STATUS_BAR_Y  (UI_SCREEN_HEIGHT - UI_BOTTOM_MARGIN - 1)


static const uiElement mesh_elements[] =
{
    { ID_SAVE_BUTTON,  0, STATUS_BAR_Y,  UI_SCREEN_WIDTH,  UI_BOTTOM_MARGIN, V(&buttons[0]) },

    { ID_PLUS10_BUTTON,  245,   40,  60, 38, V(&buttons[1]) },
    { ID_PLUS1_BUTTON,   245,   80,  60, 38, V(&buttons[2]) },
    { ID_MINUS1_BUTTON,  245,  120,  60, 38, V(&buttons[3]) },
    { ID_MINUS10_BUTTON, 245,  160,  60, 38, V(&buttons[4]) },

    { ID_WIDTH,            0,   38, 220, 33, V(&params[0]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
    { ID_HEIGHT,           0,   71, 220, 33, V(&params[1]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
    { ID_X_STEPS,          0,  104, 220, 33, V(&params[2]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
    { ID_Y_STEPS,          0,  137, 220, 33, V(&params[3]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
    { ID_NUM_PROBES,       0,  170, 220, 33, V(&params[4]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
};



static int saved_value;
static int selected_item = -1;

#define ITEM_STATE_SELECTED   0x0001
#define ITEM_STATE_DIRTY      0x0002
#define ITEM_STATE_SAVED      0x0004

uint8_t item_state[NUM_PARAMS];
uint8_t last_item_state[NUM_PARAMS];
int     last_mesh_value[NUM_PARAMS];
int     last_button_bg[NUM_BUTTONS];

static const bool allows_10[NUM_PARAMS] = {true, true, false, false, false};
static const int  mesh_min[NUM_PARAMS]  = {10, 10, 2, 2, 1};
static int  mesh_max[NUM_PARAMS]  = {1000,1000, MAX_MESH_X_STEPS,MAX_MESH_Y_STEPS, 4};

//-----------------------
// implementation
//-----------------------


dlgMeshSettings::dlgMeshSettings() :
    uiWindow(mesh_elements,sizeof(mesh_elements)/sizeof(uiElement))
{
}


void initValues()
{
    mesh_value[0] = the_mesh.getWidth();
    mesh_value[1] = the_mesh.getHeight();
    mesh_value[2] = the_mesh.getXSteps();
    mesh_value[3] = the_mesh.getYSteps();
    mesh_value[4] = the_mesh.getNumProbes();
}


void dlgMeshSettings::begin()
{
    draw_needed = true;

    initValues();

    mesh_max[0] = gStatus::getAxisMaxTravel(X_AXIS);
    mesh_max[1] = gStatus::getAxisMaxTravel(Y_AXIS);

    selected_item = -1;
    saved_value = 0;
    for (int i=0; i<NUM_PARAMS; i++)
    {
        item_state[i] = 0;
        last_item_state[i] = 0;
        last_mesh_value[NUM_PARAMS] = mesh_value[i];
    }
    for (int i=0; i<NUM_BUTTONS; i++)
    {
        buttons[i].bg = i ? COLOR_BUTTON_HIDDEN : COLOR_BUTTON_DISABLED;
        last_button_bg[i] = buttons[i].bg;
    }

    the_app.suppressStatus();
    uiWindow::begin();
    the_app.setTitle("Mesh Settings");
}




void selectItem(int i)
{
    g_debug("selectItem(%d)",i);
    if (selected_item != -1)
    {
        if (item_state[selected_item] & ITEM_STATE_DIRTY)
        {
            mesh_value[selected_item] = saved_value;
        }
        item_state[selected_item] &= ~(ITEM_STATE_SELECTED | ITEM_STATE_DIRTY);
    }

    selected_item = i;
    saved_value = mesh_value[i];
    item_state[i] |= ITEM_STATE_SELECTED;

    buttons[BTN_IDX_SAVE].bg    = COLOR_BUTTON_DISABLED;
    buttons[BTN_IDX_PLUS10].bg  = allows_10[i] ? COLOR_DARKBLUE : COLOR_BUTTON_HIDDEN;
    buttons[BTN_IDX_PLUS1].bg   = COLOR_DARKBLUE;
    buttons[BTN_IDX_MINUS1].bg  = COLOR_DARKBLUE;
    buttons[BTN_IDX_MINUS10].bg = allows_10[i] ? COLOR_DARKBLUE : COLOR_BUTTON_HIDDEN;
}


void modifyItem(int i, int inc)
{
    g_debug("modifyItem(%d,%d)",i,inc);
    mesh_value[i] += inc;
    if (mesh_value[i] < mesh_min[i])
        mesh_value[i] = mesh_min[i];
    if (mesh_value[i] > mesh_max[i])
        mesh_value[i] = mesh_max[i];

    if (mesh_value[i] == saved_value)
    {
        item_state[i] &= ~ITEM_STATE_DIRTY;
        buttons[BTN_IDX_SAVE].bg = COLOR_BUTTON_DISABLED;
    }
    else
    {
        item_state[i] |= ITEM_STATE_DIRTY;
        buttons[BTN_IDX_SAVE].bg = COLOR_DARKBLUE;
    }
}


void saveItem(int i)
{
    item_state[i] &= ~ITEM_STATE_DIRTY;
    item_state[i] |= ITEM_STATE_SAVED;

    char buf[40];
    sprintf(buf,"$mesh/%s=%d",params[i].label,mesh_value[i]);
    if (!gActions::do_setting(buf))
        return;

    // then re-get the values
    // and reselect the parameter

    initValues();
    selectItem(i);
}



void dlgMeshSettings::onButton(const uiElement *ele, bool pressed)
{
    if (pressed)       // numerical buttons happen when pressed
    {
        if (selected_item != -1)
        {
            switch (ele->id_type)
            {
                case ID_PLUS10_BUTTON:
                    modifyItem(selected_item,10);
                    break;
                case ID_PLUS1_BUTTON:
                    modifyItem(selected_item,1);
                    break;
                case ID_MINUS1_BUTTON:
                    modifyItem(selected_item,-1);
                    break;
                case ID_MINUS10_BUTTON:
                    modifyItem(selected_item,-10);
                    break;
            }
        }
    }
    else    // normal buttons happen when released
    {
        int param_num = (ele->id_type & 0xff) - FIRST_PARAM_NUMBER;
        switch (ele->id_type)
        {
            case ID_SAVE_BUTTON:
                saveItem(selected_item);
                break;

            case ID_WIDTH :
            case ID_HEIGHT :
            case ID_X_STEPS :
            case ID_Y_STEPS :
            case ID_NUM_PROBES :
                selectItem(param_num);
                break;
        }
    }
}




void dlgMeshSettings::update()
{
    for (int i=0; i<NUM_PARAMS; i++)
    {
        if (draw_needed ||
            last_item_state[i] != item_state[i] ||
            last_mesh_value[i] != mesh_value[i] )
        {
            int state = last_item_state[i] = item_state[i];
            last_mesh_value[i] = mesh_value[i];

            params[i].bg =
                state & ITEM_STATE_DIRTY ? COLOR_DARKRED :
                state & ITEM_STATE_SELECTED ? COLOR_DARKCYAN :
                state & ITEM_STATE_SAVED ? COLOR_DARKGREEN :
                COLOR_BLUE;

            int ele_idx = i + IDX_PARAMS;
            drawTypedElement(&mesh_elements[ele_idx]);
        }   // item changed
    }   // for each item

    for (int i=0; i<NUM_BUTTONS; i++)
    {
       if (draw_needed ||
           last_button_bg[i] != buttons[i].bg)
       {
            last_button_bg[i] = buttons[i].bg;
            drawTypedElement(&mesh_elements[i]);
       }
    }

    draw_needed = false;
}


#endif  // UI_WITH_MESH
