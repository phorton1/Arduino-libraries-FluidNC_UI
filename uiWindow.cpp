//-------------------------------------------------
// Generic OOP Windowing System based on TFT_eSPI
//-------------------------------------------------
// Does not use any dynamic memory.
// Allows for static construction of entire application

#include "uiWindow.h"
#include "Grbl_MinUI.h"


// virtual
void uiWindow::draw()
{
    if (!hasState(WIN_STATE_VALID | WIN_STATE_HIDDEN))
    {
        tft.fillRect(m_rect.x, m_rect.y, m_rect.w, m_rect.h, m_bg);
        addState(WIN_STATE_VALID);
    }
}



// virtual
void uiStaticText::draw()
{
    if (!hasState(WIN_STATE_VALID | WIN_STATE_HIDDEN))
    {
        uiWindow::draw();       // wasted code, should only be outline
        drawText(m_text,m_just,m_font,m_rect.x,m_rect.y,m_rect.w,m_rect.h,m_fg,m_bg);
    }
}
