//--------------------------------------
// The confirm Yes/No window
//--------------------------------------

#include "dlgConfirm.h"
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

#define MAX_CONFIRM_LINE 40


#define ID_LINE1        (0x0001 | ID_TYPE_TEXT)
#define ID_LINE2        (0x0002 | ID_TYPE_TEXT)
#define ID_YES_BUTTON   (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON  )
#define ID_NO_BUTTON    (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON  )

#define RU_SURE     "Are you sure you want to"

dlgConfirm  confirm_dlg;
static uint16_t pending_command;

//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------

char line1[MAX_CONFIRM_LINE + 1];
char line2[MAX_CONFIRM_LINE + 1];

static const uiElement confirm_elements[] =
{
    { ID_LINE1,        0,  60, 320,  25,   V(line1),  COLOR_BLACK, COLOR_WHITE, FONT_NORMAL,},
    { ID_LINE2,        0,  85, 320,  25,   V(line2),  COLOR_BLACK, COLOR_WHITE, FONT_NORMAL,},
    { ID_YES_BUTTON,  40, 130, 100,  50,   V("YES"),  COLOR_BLUE,  COLOR_WHITE, FONT_BIG,},
    { ID_NO_BUTTON,  180, 130, 100,  50,   V("NO"),   COLOR_BLUE,  COLOR_WHITE, FONT_BIG,},
};


//-----------------------
// implementation
//-----------------------

dlgConfirm::dlgConfirm() :
    uiWindow(confirm_elements,(sizeof(confirm_elements)/sizeof(uiElement)))
{}


void dlgConfirm::setConfirm(uint16_t command)
{
    strcpy(line1,"Are you sure you want to");
    if (command >= CONFIRM_COMMAND_RUN_FILE)
        strcat(line1," run");

    if (command == CONFIRM_COMMAND_RESET)
        strcpy(line2,"restart the G-Machine");
    else if (command == CONFIRM_COMMAND_REBOOT)
        strcpy(line2,"reboot the Controller");
    else if (command >= CONFIRM_COMMAND_RUN_FILE)
    {
        const char *fn = files_win.getFileToRun(command - CONFIRM_COMMAND_RUN_FILE);
        int len = strlen(fn);
        if (len > MAX_CONFIRM_LINE - 2)
            len = MAX_CONFIRM_LINE - 2;
        strncpy(line2,fn,len);
        line2[len] = 0;
    }
    strcat(line2," ?");
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

                char filename[MAX_FILENAME+1];
                strcpy(filename,files_win.getPath());
                int file_num = pending_command - CONFIRM_COMMAND_RUN_FILE;
                const char *fn = files_win.getFileToRun(file_num);

                if (strlen(filename) + strlen(fn) + 1 >= MAX_FILENAME)
                {
                    errorMsg("path too long to run");
                    return;
                }
                if (strcmp(filename,"/"))
                    strcat(filename,"/");
                strcat(filename,fn);

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
