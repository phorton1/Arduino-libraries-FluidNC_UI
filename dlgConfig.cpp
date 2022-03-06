//-------------------------------------------------
// A dialog to select a config.yaml from the SPFFS
//-------------------------------------------------

#include "dlgConfig.h"
#include "dlgConfirm.h"
#include <gActions.h>   // FluidNC_extensions
#include <SettingsDefinitions.h>     // FluidNC
// #include <GCode.h>      // a weird way to include the proper FluidNC Config.h
#include <SPIFFS.h>
#include <FS.h>


dlgConfig dlg_config;


//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------

#define MAX_CONFIG           5
#define MAX_CONFIG_FILENAME  31

static char config_filenames[MAX_CONFIG][MAX_CONFIG_FILENAME+1];

#define ID_CONFIG_0  (0x0000 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_CONFIG_1  (0x0001 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_CONFIG_2  (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_CONFIG_3  (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_CONFIG_4  (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)

static uiMutable configs[MAX_CONFIG] =
{
    { config_filenames[0],  COLOR_BUTTON_HIDDEN,   COLOR_WHITE, FONT_SMALL },
    { config_filenames[1],  COLOR_BUTTON_HIDDEN,   COLOR_WHITE, FONT_SMALL },
    { config_filenames[2],  COLOR_BUTTON_HIDDEN,   COLOR_WHITE, FONT_SMALL },
    { config_filenames[3],  COLOR_BUTTON_HIDDEN,   COLOR_WHITE, FONT_SMALL },
    { config_filenames[4],  COLOR_BUTTON_HIDDEN,   COLOR_WHITE, FONT_SMALL },
};


static const uiElement config_elements[] =
{
    { ID_CONFIG_0,     20,   35,  280, 33, V(&configs[0]) },
    { ID_CONFIG_1,     20,   69,  280, 33, V(&configs[1]) },
    { ID_CONFIG_2,     20,  103,  280, 33, V(&configs[2]) },
    { ID_CONFIG_3,     20,  137,  280, 33, V(&configs[3]) },
    { ID_CONFIG_4,     20,  171,  280, 33, V(&configs[4]) },
};



//-----------------------
// implementation
//-----------------------


dlgConfig::dlgConfig() :
    uiWindow(config_elements,sizeof(config_elements)/sizeof(uiElement))
{
}



void dlgConfig::begin()
    // set the app title to the current name and
    // populate the list of existing config filenames
{
    the_app.setTitle(config_filename->getStringValue());
        // get the current config_filename from FluidNC::SettingsDefinitions.h extern'd "config_filename"

    for (int i=0; i<MAX_CONFIG; i++)
    {
        configs[i].bg = COLOR_BUTTON_HIDDEN;
    }

    int num_config = 0;
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (num_config < MAX_CONFIG && file)
    {
        const char* name = file.name();

        if (!file.isDirectory())
        {
            int len = strlen(name);
            int pos = len-1;
            if (*name == '/') name++;
            while (pos && name[pos] != '.') pos--;

            // g_debug("filename=%s ext=%s",name,&name[pos]);

            if (pos && !strcmp(&name[pos],".yaml"))
            {
                if (len > MAX_CONFIG_FILENAME)
                {
                    g_error("Config filename %s is longer than MAX_CONFIG_FILENAME=%d",name,MAX_CONFIG_FILENAME);
                }
                else
                {
                    configs[num_config].bg = COLOR_BLUE;
                    strcpy(config_filenames[num_config++],name);
                }
            }
        }
        file = root.openNextFile();
    }
    root.close();
    uiWindow::begin();
}


const char *dlgConfig::getConfigFilename(int config_num)
{
    return config_filenames[config_num];
}


void dlgConfig::setConfigFilename(int config_num)
{
    g_info("CHANGING CONFIG to %s",config_filenames[config_num]);

    #define CMD_BASE  "$config/filename="
    char buf[strlen(CMD_BASE) + MAX_CONFIG_FILENAME + 4];
    strcpy(buf,CMD_BASE);
    strcat(buf,config_filenames[config_num]);
    strcat(buf,"\r\n");
    gActions::pushGrblText(buf);
    delay(1000);

    gActions::g_reset();
    ESP.restart();
    while (1) {}
}



void dlgConfig::onButton(const uiElement *ele, bool pressed)
{
    if (!pressed)    // normal buttons happen when released
    {
        int config_num = ele->id_type & 0x0f;   // could be upto 0xff
        dlg_confirm.setConfirm(CONFIRM_COMMAND_SET_CONFIG + config_num);
        the_app.openWindow(&dlg_confirm);
    }
}
