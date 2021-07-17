//--------------------------------------
// GRBL application specific classes
//--------------------------------------
// gApplication, gTitleBar, gContentArea, and gStatusBar


#include "gApp.h"
#include "Grbl_MinUI.h"


gApplication the_app;


gTitleBar::gTitleBar() :
    uiWindow     (the_app, JUST_LEFT, 0, 0, UI_SCREEN_WIDTH, UI_TOP_MARGIN, COLOR_GREEN, COLOR_WHITE),
        // the parent window is generally unused
    m_main_button(*this, "MAIN",  JUST_CENTER, FONT_BIG,  0,                    0, UI_MAIN_BUTTON_WIDTH,                                      UI_TOP_MARGIN, COLOR_BLUE,     COLOR_WHITE),
        // sheesh, FONT_BIG has descenders, but no ascenders, so it does not center vertically correctly,
        // so, what? what a fucking mess
    m_title      (*this, "a very long title to test clipping just in case", JUST_CENTER, FONT_NORMAL, UI_MAIN_BUTTON_WIDTH, 0, UI_SCREEN_WIDTH-UI_MAIN_BUTTON_WIDTH-2*UI_INDICATOR_WIDTH, UI_TOP_MARGIN, COLOR_DARKBLUE, COLOR_WHITE)
{
}


// virtual
void gTitleBar::draw()
{
    // uiWindow::draw();
    m_main_button.draw();
    m_title.draw();
}



gStatusBar::gStatusBar() :
    uiWindow(the_app,0,UI_SCREEN_HEIGHT-UI_BOTTOM_MARGIN,UI_SCREEN_WIDTH,UI_BOTTOM_MARGIN,COLOR_DARKBLUE,COLOR_WHITE)
{
}


gApplication::gApplication()
{
    m_rect.assign(0,0,UI_SCREEN_WIDTH,UI_SCREEN_HEIGHT);
    m_bg = COLOR_BLACK;
    m_fg = COLOR_WHITE;
}



// virtual
void gApplication::update()
{
    g_status.updateStatus();
    uiWindow::draw();
    m_title_bar.draw();
    m_status_bar.draw();
}