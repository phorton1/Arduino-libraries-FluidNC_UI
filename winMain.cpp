//--------------------------------------
// The 'main' idle/nav window
//--------------------------------------

#include "winMain.h"

#ifdef WITH_APPLICATION

    #include "dlgMsg.h"

    #ifdef WITH_GRBL
        #include <SDCard.h>
        #include <Serial.h>
        #include <Machine/MachineConfig.h>
        #include <SD.h>
    #endif

    winMain main_win;

    //----------------------------------------------------------------------
    // WINDOW DEFINITION
    //----------------------------------------------------------------------

    #define IDX_TEXT_FIELD   0
    #define ID_SOME_BUTTON1     (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
    #define ID_SOME_BUTTON2     (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)

    static char some_text[UI_MAX_TITLE] = "variable text";
    static char button_text[UI_MAX_BUTTON] = "BLECH";

    static uiMutable mutable_button = {
        "BLEY",
        COLOR_YELLOW,
        COLOR_PURPLE,
        FONT_SMALL
    };

    static const uiElement idle_elements[] = {

        { ID_TYPE_TEXT,       0,  40, 320, 20, V(some_text),           COLOR_BLACK,     COLOR_WHITE, FONT_NORMAL, JUST_CENTER },
        { ID_SOME_BUTTON1,   20,  70, 120, 90, V(button_text),         COLOR_DARKGREEN, COLOR_WHITE, FONT_BIG,    JUST_CENTER },
        { ID_SOME_BUTTON2,  160,  70, 120, 90, V(&mutable_button),     },
        { ID_TYPE_TEXT,       0, 170, 320, 20, V("this is some text"), COLOR_BLACK,     COLOR_WHITE, FONT_NORMAL, JUST_CENTER },

    };


    //----------------------------------
    // implementation
    //----------------------------------

    winMain::winMain() :
        uiWindow(idle_elements,(sizeof(idle_elements)/sizeof(uiElement)))
    {}


    void winMain::onButton(const uiElement *ele, bool pressed)
        // called before drawElement
    {
        g_debug("winMain::onButton(%04x) pressed=%d",ele->id_type,pressed);

        if (!pressed)
        {
            if (ele->id_type == ID_SOME_BUTTON1)
            {
                static int counter = 0;
                sprintf(button_text,"BLAH%d",counter++);
                    // drawElement will be called after onButton(false)
                sprintf(some_text, "counter = %d",counter);
                drawTypedElement(&m_elements[IDX_TEXT_FIELD]);
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

                #ifdef WITH_GRBL
                    SDCard *sdCard = config->_sdCard;
                    // g_debug("winMain testing SD Card");
                    if (sdCard)
                    {
                        // g_debug("winMain starting SD Card");
                        if (sdCard->get_state(true) == SDCard::State::Idle)
                        {
                            // g_debug("winMain opening ruler.g");
                            if (sdCard->openFile(SD,"/ruler.g"))
                            {
                                // g_debug("winMain running ruler.g");
                                sdCard->_client = CLIENT_ALL;
                                sdCard->_readyNext = true;
                            }
                            else
                                errorMsg("Could not open ruler.g");
                        }
                        else
                            errorMsg("Could not get SDCard");
                    }
                    else
                        errorMsg("NO sdCard");

                #endif
            }
        }
    }

#endif  // WITH_APPLICATION
