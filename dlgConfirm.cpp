//--------------------------------------
// The confirm Yes/No window
//--------------------------------------

#include "dlgConfirm.h"
#include "winFiles.h"
#include "winMain.h"
#include "dlgMsg.h"
#include <gStatus.h>    // FluidNC_extensions
#include <gActions.h>   // FluidNC_extensions


#define MAX_CONFIRM_LINE 40

#define ID_LINE1        (0x0001 | ID_TYPE_TEXT)
#define ID_LINE2        (0x0002 | ID_TYPE_TEXT)
#define ID_YES_BUTTON   (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON  )
#define ID_NO_BUTTON    (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON  )

#define RU_SURE     "Are you sure you want to"


dlgConfirm  dlg_confirm;
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
                the_app.clearLastJobState();
                gActions::realtime_command(Cmd::Reset);

                // The UI version of "RESET" goes one step further,
                // and if we are in an alarm, ALSO issues the $X command
                // after a short delay to clear the alarm as well

                if (g_status.getLastAlarm())
                {
                    the_app.setTitle("");
                    delay(500);
                    gActions::pushGrblText("$X\r\n");
                }

                // end this window, so underlying modal will also be ended
                // endModal() will do nothing if the current window is not
                // modal, so it's alright to call this two times even if
                // there's only one modal window on the stack.

                the_app.endModal();
            }
            else if (pending_command == CONFIRM_COMMAND_REBOOT)
            {
                g_debug("gApplication estarting the ESP32!!");
                delay(500);

                // ESP.restart() works better after a call to mc_reset()
                // otherwise steppers don't work after reboot

                gActions::g_reset();
                ESP.restart();
                while (1) {}
            }
            else if (pending_command >= CONFIRM_COMMAND_RUN_FILE)
            {
                #define MAX_FILENAME  128
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

                // the application will end the files window
                // and return to the main window on the job_state change

                g_debug("dlgConfirm running %s",filename);
                if (gActions::startSDJob(filename))
                	the_app.setBaseWindow(&main_win);

            }
        }
        the_app.endModal();
    }
}
