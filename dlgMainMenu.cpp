//--------------------------------------
// The confirm Yes/No window
//--------------------------------------

#include "dlgMainMenu.h"
#include "winMain.h"
#include "winFiles.h"

dlgMainMenu main_menu;

//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------

#define NUM_APP_WINDOWS     2

#define ID_BUTTON1     (0x0001 | ID_TYPE_TEXT | ID_TYPE_BUTTON)
#define ID_BUTTON2     (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON)

// for now I am defining a bunch of full mutables
// probably want something different

static uiWindow *app_windows[NUM_APP_WINDOWS] = {
    &main_win,
    &files_win };


static char button_text[NUM_APP_WINDOWS][UI_MAX_BUTTON];

static const uiElement menu_elements[] =
{
    { ID_BUTTON1,     0,  35, 90, 35, V(button_text[0]), COLOR_BLUE,  COLOR_WHITE, FONT_BIG, JUST_CENTER },
    { ID_BUTTON2,     0,  70, 90, 35, V(button_text[1]), COLOR_BLUE,  COLOR_WHITE, FONT_BIG, JUST_CENTER },
};




//-----------------------
// implementation
//-----------------------

dlgMainMenu::dlgMainMenu() :
    uiWindow(menu_elements,4)
{}

void dlgMainMenu::begin()
{
    const char *app_text = the_app.getAppButtonText();
    app_text = strstr(app_text,"%") ? "MAIN" : app_text;
        // sublimate % button to "main"

    m_num_elements = 0;
    for (int i=0; i<NUM_APP_WINDOWS; i++)
    {
        const char *text = app_windows[i]->getMenuLabel();


        if (strcmp(app_text,text))
        {
            strcpy(button_text[m_num_elements++],text);
        }
    }
    drawTypedElements();
}


void dlgMainMenu::onButton(const uiElement *ele, bool pressed)
{
    if (!pressed)
    {
        const char *text = (const char *) ele->param;
        for (int i=0; i<NUM_APP_WINDOWS; i++)
        {
            uiWindow *win = app_windows[i];
            if (!strcmp(text,win->getMenuLabel()))
            {
                the_app.setBaseWindow(win);
                return;
            }
        }
    }
}
