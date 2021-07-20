//--------------------------------------
// The main Idle window
//--------------------------------------

#include "winIdle.h"
#include "Grbl_MinUI.h"    // for debug_serial()



#define ID_SOME_BUTTON1     (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_SOME_BUTTON2     (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)


char some_text[UI_MAX_TITLE] = "variable text";
char button_text[UI_MAX_BUTTON] = "BLECH";


uiMutable mutable_button = {
    "BLEY",
    COLOR_YELLOW,
    COLOR_PURPLE,
    FONT_SMALL
};


//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------


const uiElement idle_elements[] = {

    { ID_TYPE_TEXT,       0,  40, 320, 20, V(some_text),           COLOR_BLACK,     COLOR_WHITE, FONT_NORMAL, JUST_CENTER },
    { ID_SOME_BUTTON1,   20,  70, 120, 90, V(button_text),         COLOR_DARKGREEN, COLOR_WHITE, FONT_BIG,    JUST_CENTER },
    { ID_SOME_BUTTON2,  160,  70, 120, 90, V(&mutable_button),     },
    { ID_TYPE_TEXT,       0, 170, 320, 20, V("this is some text"), COLOR_BLACK,     COLOR_WHITE, FONT_NORMAL, JUST_CENTER },

};


winIdle::winIdle() :
    uiWindow(idle_elements,(sizeof(idle_elements)/sizeof(uiElement)))
{}


//----------------------------------
// methods
//----------------------------------

#if !TEST_STANDALONE_UI
	// #include <Config.h>
	// #include <Grbl.h>
	#include <SDCard.h>
	// #include <Report.h>
	#include <Machine/MachineConfig.h>
    #include <SD.h>
#endif



void winIdle::onButton(const uiElement *ele, bool pressed)
    // called before drawElement
{
    debug_serial("winIdle::onButton(%04x) pressed=%d",ele->id_type,pressed);

    if (!pressed)
    {
        if (ele->id_type == ID_SOME_BUTTON1)
        {
            static int counter = 0;
            sprintf(button_text,"BLAH%d",counter++);
                // drawElement will be called after onButton(false)
            sprintf(some_text, "counter = %d",counter);
            drawTypedElement(&m_elements[0]);
                // just happen to know the index of the text field
                // that references some_text
        }
        else if (ele->id_type == ID_SOME_BUTTON2)
        {
            static bool state = false;
            state = !state;
            mutable_button.text = state ? "RAW" : "MEAT";
            mutable_button.bg   = state ? COLOR_RED : COLOR_BLUE;
            mutable_button.fg   = state ? COLOR_BLACK : COLOR_WHITE;
            mutable_button.font = state ? FONT_MONO : FONT_BIG;
                // drawElement will be called after onButton(false)

            #if !TEST_STANDALONE_UI
                SDCard *sdCard = config->_sdCard;
                debug_serial("winIdle testing SD Card");
                if (sdCard)
                {
                    debug_serial("winIdle starting SD Card");
                    if (sdCard->get_state(true) == SDCard::State::Idle)
                    {
                        debug_serial("winIdle opening ruler.g");
                        if (sdCard->openFile(SD,"/ruler.g"))
                        {
                            debug_serial("winIdle running ruler.g");
                            sdCard->_client = CLIENT_ALL;
                            sdCard->_readyNext = true;
                        }
                    }
                }
            #endif
        }
    }
}
