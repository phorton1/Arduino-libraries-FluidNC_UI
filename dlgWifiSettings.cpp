//-----------------------------------------
// A dialog for WifiSettings
//-----------------------------------------

#include "dlgWifiSettings.h"

dlgWifiSettings dlg_wifi_settings;


//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------

#define ID_STA_MODE         (ID_TYPE_TEXT | ID_TYPE_DUAL)
#define ID_WIFI_NAME        (ID_TYPE_TEXT | ID_TYPE_DUAL)
#define ID_IP_ADDR          (ID_TYPE_TEXT | ID_TYPE_DUAL)

static char station_mode[8];
static char wifi_name[30];
static char ip_addr[30];


static uiDualElement params[] =
{
    { ELEMENT_TYPE_STR,    COLOR_BLUE, 120, 170, "Station Mode",   &station_mode, FONT_SMALL },
    { ELEMENT_TYPE_STR,    COLOR_BLUE, 120, 170, "Name",           &wifi_name,    FONT_SMALL },
    { ELEMENT_TYPE_STR,    COLOR_BLUE, 120, 170, "IP addr",        &ip_addr,      FONT_SMALL },
};


static const uiElement wifi_elements[] =
{
    { ID_STA_MODE,     5,   52, 310, 33, V(&params[0]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
    { ID_WIFI_NAME,    5,   87, 310, 33, V(&params[1]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
    { ID_IP_ADDR,      5,  122, 310, 33, V(&params[2]), COLOR_BLUE,  COLOR_WHITE, FONT_SMALL, JUST_LEFT },
};



//-----------------------
// implementation
//-----------------------


dlgWifiSettings::dlgWifiSettings() :
    uiWindow(wifi_elements,sizeof(wifi_elements)/sizeof(uiElement))
{
}




void dlgWifiSettings::begin()
{
    draw_needed = true;

    uint8_t mode = gStatus::getWifiStationMode();
    if (mode == 1)
        strcpy(station_mode,"STA");
    else if (mode == 2)
        strcpy(station_mode,"AP");
    else if (mode == 3)
        strcpy(station_mode,"AP/STA");
    else
        strcpy(station_mode,"NONE");

    strcpy(wifi_name,gStatus::getWifiName());
    strcpy(ip_addr,gStatus::getIPAddress());

    the_app.setTitle("Wifi Settings");
    uiWindow::begin();
}



