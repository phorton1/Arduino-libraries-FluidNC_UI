//-----------------------------------------
// A dialog to modify realtime feed rates
//-----------------------------------------

#include "dlgFeedRates.h"

#include <gActions.h>   // FluidNC_extensions
#include <GCode.h>      // a weird way to include the proper FluidNC Config.h

#ifdef UI_WITH_MESH
    #include <Mesh.h>   // FluidNC_extensions
#endif


dlgFeedRates dlg_feed_rates;


//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------

#define NUM_BUTTONS          5

#define ID_BUTTON_0  (0x0001 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_BUTTON_1  (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_BUTTON_2  (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_BUTTON_3  (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_BUTTON_4  (0x0005 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)

#define FIRST_PARAM_NUMBER 0x0011

#define ID_FEED_RATE       ((FIRST_PARAM_NUMBER+0) | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_NO_BORDER | ID_TYPE_DUAL)
#define ID_RAPID_RATE      ((FIRST_PARAM_NUMBER+1) | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_NO_BORDER | ID_TYPE_DUAL)
#define ID_SPINDLE_RATE    ((FIRST_PARAM_NUMBER+2) | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_NO_BORDER | ID_TYPE_DUAL)
#define ID_LIVE_Z          ((FIRST_PARAM_NUMBER+3) | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_NO_BORDER | ID_TYPE_DUAL)

static char button_text[NUM_BUTTONS][8];

static uiMutable buttons[NUM_BUTTONS] =
{
    { button_text[0],  COLOR_BLUE,   COLOR_WHITE, FONT_SMALL },
    { button_text[1],  COLOR_BLUE,   COLOR_WHITE, FONT_SMALL },
    { button_text[2],  COLOR_BLUE,   COLOR_WHITE, FONT_SMALL },
    { button_text[3],  COLOR_BLUE,   COLOR_WHITE, FONT_SMALL },
    { button_text[4],  COLOR_BLUE,   COLOR_WHITE, FONT_SMALL },
};


#ifdef UI_WITH_MESH
    #define NUM_PARAMS  4
    static float last_live_z;
#else
    #define NUM_PARAMS  3
#endif

static int override_value[NUM_PARAMS];  // last one is live_z * 1000

static uiDualElement params[NUM_PARAMS] =
{
    { ELEMENT_TYPE_INT,    COLOR_BLUE, 190, 70, "Feed Rate",    &override_value[0], FONT_SMALL },
    { ELEMENT_TYPE_INT,    COLOR_BLUE, 190, 70, "Rapid Feed",   &override_value[1], FONT_SMALL },
    { ELEMENT_TYPE_INT,    COLOR_BLUE, 190, 70, "Spindle",      &override_value[2], FONT_SMALL },
#ifdef UI_WITH_MESH
    { ELEMENT_TYPE_FLOAT3, COLOR_BLUE, 190, 70, "Live Z",       &last_live_z,         FONT_SMALL },
#endif
};


#define IDX_PARAMS  5


static const uiElement override_elements[] =
{
    { ID_BUTTON_0,      245,   35,  60, 33, V(&buttons[0]) },
    { ID_BUTTON_1,      245,   69,  60, 33, V(&buttons[1]) },
    { ID_BUTTON_2,      245,  103,  60, 33, V(&buttons[2]) },
    { ID_BUTTON_3,      245,  137,  60, 33, V(&buttons[3]) },
    { ID_BUTTON_4,      245,  171,  60, 33, V(&buttons[4]) },
    { ID_FEED_RATE,       0,   52, 220, 33, V(&params[0]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
    { ID_RAPID_RATE,      0,   87, 220, 33, V(&params[1]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
    { ID_SPINDLE_RATE,    0,  122, 220, 33, V(&params[2]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
#ifdef UI_WITH_MESH
    { ID_LIVE_Z,          0,  157, 220, 33, V(&params[3]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
#endif
};


static int selected_item = -1;

#define ITEM_STATE_SELECTED   0x0001

static uint8_t item_state[NUM_PARAMS];
static uint8_t last_item_state[NUM_PARAMS];
static int last_override_value[NUM_PARAMS];
static int last_button_bg[NUM_BUTTONS];




//-----------------------
// implementation
//-----------------------


dlgFeedRates::dlgFeedRates() :
    uiWindow(override_elements,sizeof(override_elements)/sizeof(uiElement))
{
}


void dlgFeedRates::initValues()
{
    override_value[PARAM_IDX_FEED_RATE] = g_status.getFeedOverride();
    override_value[PARAM_IDX_RAPID_RATE] = g_status.getRapidFeedOverride();
    override_value[PARAM_IDX_SPINDLE_RATE] = g_status.getSpindleOverride();
    #ifdef UI_WITH_MESH
        last_live_z = the_mesh.getLiveZ();
        override_value[PARAM_IDX_LIVE_Z] = 1000 * last_live_z;
    #endif
}


void dlgFeedRates::setSelectedItem(int param_num)
{
    selected_item = param_num;
}


void dlgFeedRates::begin()
{
    initValues();
    draw_needed = true;
    for (int i=0; i<NUM_PARAMS; i++)
    {
        item_state[i] = 0;
        last_item_state[i] = 0;
        last_override_value[i] = 0;
    }
    for (int i=0; i<NUM_BUTTONS; i++)
    {
        last_button_bg[i] = buttons[i].bg;
    }

    the_app.setTitle("Feed Rates");
    selectItem(selected_item);
    // the_app.suppressStatus();
    uiWindow::begin();
}




void dlgFeedRates::selectItem(int param_num)
{
    g_debug("selectItem(%d)",param_num);
    if (selected_item != -1)
        item_state[selected_item] &= ~ITEM_STATE_SELECTED;
    selected_item = param_num;
    item_state[param_num] |= ITEM_STATE_SELECTED;

    for (int i=0; i<NUM_BUTTONS; i++)
    {
        buttons[i].bg = COLOR_BLUE;
        buttons[i].font = param_num == PARAM_IDX_LIVE_Z ? FONT_MONO : FONT_SMALL;
    }

    if (param_num == PARAM_IDX_FEED_RATE)
    {
        sprintf(button_text[0],"+%d",FeedOverride::CoarseIncrement);
        sprintf(button_text[1],"+%d",FeedOverride::FineIncrement);
        sprintf(button_text[2],"%d", FeedOverride::Default);
        sprintf(button_text[3],"-%d",FeedOverride::FineIncrement);
        sprintf(button_text[4],"-%d",FeedOverride::CoarseIncrement);
    }
    else if (param_num == PARAM_IDX_RAPID_RATE)
    {
        buttons[0].bg = COLOR_BUTTON_HIDDEN;
        sprintf(button_text[1],"%d",RapidOverride::Default);
        sprintf(button_text[2],"%d",RapidOverride::Medium);
        sprintf(button_text[3],"%d",RapidOverride::Low);
    }
    else if (param_num == PARAM_IDX_SPINDLE_RATE)
    {
        sprintf(button_text[0],"+%d",SpindleSpeedOverride::CoarseIncrement);
        sprintf(button_text[1],"+%d",SpindleSpeedOverride::FineIncrement);
        sprintf(button_text[2],"%d", SpindleSpeedOverride::Default);
        sprintf(button_text[3],"-%d",SpindleSpeedOverride::FineIncrement);
        sprintf(button_text[4],"-%d",SpindleSpeedOverride::CoarseIncrement);
    }
    #ifdef UI_WITH_MESH
        else if (param_num == PARAM_IDX_LIVE_Z)
        {
            sprintf(button_text[0],"+%0.3f",LIVE_Z_COARSE);
            sprintf(button_text[1],"+%0.3f",LIVE_Z_FINE);
            sprintf(button_text[2],"%0.0f",LIVE_Z_DEFAULT);
            sprintf(button_text[3],"-%0.3f",LIVE_Z_FINE);
            sprintf(button_text[4],"-%0.3f",LIVE_Z_COARSE);
        }
    #endif

    draw_needed = true;
}


void dlgFeedRates::modifyItem(int param_num, int button_num)
{
    g_debug("dlgFeedRates::modifyItem(%d,%d)",param_num,button_num);
    if (param_num == PARAM_IDX_FEED_RATE)
    {
        switch (button_num)
        {
            case 0: gActions::realtime_command( Cmd::FeedOvrCoarsePlus );    break;
            case 1: gActions::realtime_command( Cmd::FeedOvrFinePlus );      break;
            case 2: gActions::realtime_command( Cmd::FeedOvrReset );         break;
            case 3: gActions::realtime_command( Cmd::FeedOvrFineMinus );     break;
            case 4: gActions::realtime_command( Cmd::FeedOvrCoarseMinus );   break;
        }
    }
    else if (param_num == PARAM_IDX_RAPID_RATE)
    {
        switch (button_num)
        {
            case 1: gActions::realtime_command( Cmd::RapidOvrReset );   break;
            case 2: gActions::realtime_command( Cmd::RapidOvrMedium );  break;
            case 3: gActions::realtime_command( Cmd::RapidOvrLow );     break;
        }
    }
    else     if (param_num == PARAM_IDX_FEED_RATE)
    {
        switch (button_num)
        {
            case 0: gActions::realtime_command( Cmd::FeedOvrCoarsePlus );    break;
            case 1: gActions::realtime_command( Cmd::FeedOvrFinePlus );      break;
            case 2: gActions::realtime_command( Cmd::FeedOvrReset );         break;
            case 3: gActions::realtime_command( Cmd::FeedOvrFineMinus );     break;
            case 4: gActions::realtime_command( Cmd::FeedOvrCoarseMinus );   break;
        }
    }
    else if (param_num == PARAM_IDX_SPINDLE_RATE)
    {
        switch (button_num)
        {
            case 0: gActions::realtime_command( Cmd::SpindleOvrCoarsePlus );   break;
            case 1: gActions::realtime_command( Cmd::SpindleOvrFinePlus );     break;
            case 2: gActions::realtime_command( Cmd::SpindleOvrReset );        break;
            case 3: gActions::realtime_command( Cmd::SpindleOvrFineMinus );    break;
            case 4: gActions::realtime_command( Cmd::SpindleOvrCoarseMinus );  break;
        }
    }
    #ifdef UI_WITH_MESH
        else if (param_num == PARAM_IDX_LIVE_Z)
        {
            switch (button_num)
            {
                case 0: gActions::realtime_command( static_cast<Cmd>(CMD_LIVE_Z_PLUS_COARSE) );   break;
                case 1: gActions::realtime_command( static_cast<Cmd>(CMD_LIVE_Z_PLUS_FINE) );     break;
                case 2: gActions::realtime_command( static_cast<Cmd>(CMD_LIVE_Z_RESET) );         break;
                case 3: gActions::realtime_command( static_cast<Cmd>(CMD_LIVE_Z_MINUS_FINE) );    break;
                case 4: gActions::realtime_command( static_cast<Cmd>(CMD_LIVE_Z_MINUS_COARSE) );  break;
            }
        }
    #endif

}





void dlgFeedRates::onButton(const uiElement *ele, bool pressed)
{
    if (pressed)       // numerical buttons happen when pressed
    {
        if (selected_item != -1)
        {
            switch (ele->id_type)
            {
                case ID_BUTTON_0:
                    modifyItem(selected_item,0);
                    break;
                case ID_BUTTON_1:
                    modifyItem(selected_item,1);
                    break;
                case ID_BUTTON_2:
                    modifyItem(selected_item,2);
                    break;
                case ID_BUTTON_3:
                    modifyItem(selected_item,3);
                    break;
                case ID_BUTTON_4:
                    modifyItem(selected_item,4);
                    break;
            }
        }
    }
    else    // normal buttons happen when released
    {
        int param_num = (ele->id_type & 0xff) - FIRST_PARAM_NUMBER;
        switch (ele->id_type)
        {
            case ID_FEED_RATE :
            case ID_RAPID_RATE :
            case ID_SPINDLE_RATE :
            case ID_LIVE_Z :
                selectItem(param_num);
                break;
        }
    }
}




void dlgFeedRates::update()
{
    initValues();

    for (int i=0; i<NUM_PARAMS; i++)
    {
        if (draw_needed ||
            last_item_state[i] != item_state[i] ||
            last_override_value[i] != override_value[i] )
        {
            int state = last_item_state[i] = item_state[i];
            last_override_value[i] = override_value[i];

            params[i].bg =
                state & ITEM_STATE_SELECTED ? COLOR_DARKCYAN :
                COLOR_BLUE;

            int ele_idx = i + IDX_PARAMS;
            drawTypedElement(&override_elements[ele_idx]);

        }   // item changed
    }   // for each item

    for (int i=0; i<NUM_BUTTONS; i++)
    {
       if (draw_needed ||
           last_button_bg[i] != buttons[i].bg)
       {
            last_button_bg[i] = buttons[i].bg;
            drawTypedElement(&override_elements[i]);
       }
    }

    draw_needed = false;
}
