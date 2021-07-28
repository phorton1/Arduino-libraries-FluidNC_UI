//--------------------------------------
// The confirm Yes/No window
//--------------------------------------

#include "dlgConfirm.h"

#ifdef WITH_APPLICATION

    #include "winFiles.h"
    #include "dlgMsg.h"
    #include "winBusy.h"


    #ifdef WITH_GRBL
        #include <SD.h>
		#include <Grbl.h>
		#include <SDCard.h>
		#include <Report.h>
		#include <Machine/MachineConfig.h>
    #endif


    #define ID_LINE1        (0x0001 | ID_TYPE_TEXT)
    #define ID_LINE2        (0x0002 | ID_TYPE_TEXT | ID_TYPE_MUTABLE )
    #define ID_YES_BUTTON   (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON  )
    #define ID_NO_BUTTON    (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON  )

    #define RU_SURE     "Are you sure you want to"

    dlgConfirm  confirm_dlg;
    static uint16_t pending_command;

    //----------------------------------------------------------------------
    // WINDOW DEFINITION
    //----------------------------------------------------------------------

    static uiMutable confirm_msg = {
        "",
        COLOR_BLACK,
        COLOR_WHITE,
        FONT_NORMAL,
    };

    static const uiElement confirm_elements[] =
    {
        { ID_LINE1,        0,  60, 320,  25,   V(RU_SURE),      COLOR_BLACK, COLOR_WHITE, FONT_NORMAL,},
        { ID_LINE2,        0,  85, 320,  25,   V(&confirm_msg), },
        { ID_YES_BUTTON,  40, 130, 100,  50,   V("YES"),        COLOR_BLUE,  COLOR_WHITE, FONT_BIG,},
        { ID_NO_BUTTON,  180, 130, 100,  50,   V("NO"),         COLOR_BLUE,  COLOR_WHITE, FONT_BIG,},
    };


    //-----------------------
    // implementation
    //-----------------------

    dlgConfirm::dlgConfirm() :
        uiWindow(confirm_elements,(sizeof(confirm_elements)/sizeof(uiElement)))
    {}


    void dlgConfirm::setConfirm(uint16_t command)
    {
        confirm_msg.text =
            command >= CONFIRM_COMMAND_RUN_FILE ?
                files_win.getFileQuestion(command - CONFIRM_COMMAND_RUN_FILE) :
            command == CONFIRM_COMMAND_RESET ? "restart the G-Machine?" :
            command == CONFIRM_COMMAND_REBOOT ? "reboot the Controller?" :
            "UNKNOWN CONFIRM COMMAND";
         pending_command = command;
    }


    void dlgConfirm::onButton(const uiElement *ele, bool pressed)
        // called before drawElement
    {
        if (!pressed)
        {
            if (ele->id_type == ID_YES_BUTTON)
            {
                if (pending_command == CONFIRM_COMMAND_RESET)
                {
                    the_app.setTitle("");
                     #ifdef WITH_GRBL
                        execute_realtime_command(Cmd::Reset,CLIENT_ALL);
                    #endif
                }
                else if (pending_command == CONFIRM_COMMAND_REBOOT)
                {
                    g_debug("gApplication estarting the ESP32!!");
                    delay(500);
                    ESP.restart();
                    while (1) {}
                }
                else if (pending_command >= CONFIRM_COMMAND_RUN_FILE)
                {
                    int file_num = pending_command - CONFIRM_COMMAND_RUN_FILE;
                    const char *filename = files_win.getFileToRun(file_num);
                    g_debug("dlgConfirm running %s",filename);
                    #ifdef WITH_GRBL

                        // all access to Grbl_Esp32 should be encapsulated in gStatus

                        SDCard *sdCard = config->_sdCard;
                        // g_debug("winMain testing SD Card");
                        if (sdCard && sdCard->get_state(true) == SDCard::State::Idle)
                        {
                            if (sdCard->openFile(SD,filename))
                            {
                                // g_debug("winMain running ruler.g");
                                sdCard->_client = CLIENT_ALL;
                                sdCard->_readyNext = true;
                                the_app.setBaseWindow(&busy_win);
                            }
                            else
                                errorMsg("Could not open file");
                        }
                        else
                            errorMsg("Could not get SDCard");
                    #endif

                }
            }
            the_app.endModal();
        }
    }


#endif  // WITH_APPLICATION
