//--------------------------------------
// uiWindow
//--------------------------------------
// A simple non-virtual class that can be stored in PROGMEM

#pragma once

#include "uiWindow.h"
#include "Grbl_MinUI.h" // for debug_serial()


#define DEBOUNCE_TIME  100
#define PRESSURE_THRESHOLD  100		// was 600



bool uiWindow::g_pressed = false;
int  uiWindow::g_press_x = 0;
int  uiWindow::g_press_y = 0;
uint32_t uiWindow::g_debounce_time = 0;
const uiElement *uiWindow::g_win_pressed = NULL;
uiWindow *uiWindow::g_window_pressed = NULL;



bool uiWindow::hitTest()
{
    if (g_pressed && !g_win_pressed)
    {
        for (uint16_t i=0; i<m_num_elements; i++)
        {
            const uiElement *ele =&m_elements[i];
            if (ele->id_type & ID_TYPE_BUTTON &&
                g_press_x >= ele->x &&
                g_press_x <  ele->x + ele->w &&
                g_press_y >= ele->y &&
                g_press_y <  ele->y + ele->h)
            {
                uint16_t bg = ele->id_type & ID_TYPE_MUTABLE ?
                    ((uiMutable *) ele->param)->bg : ele->bg;
                if (bg != COLOR_BUTTON_DISABLED)
                {
                    g_win_pressed = ele;
                    g_window_pressed = this;
                    // debug_serial("hitTest(%08x)  onButton(true) %04x at (%d,%d,%d,%)",this,g_win_pressed->id_type,g_win_pressed->x,g_win_pressed->y,g_win_pressed->w,g_win_pressed->h);
                    onButton(g_win_pressed,true);
                    drawTypedElement(g_win_pressed,true);
                }
                return true;
            }
        }
    }
    return false;
}


void uiWindow::updateTouch()
{
	uint32_t now = millis();
	uint16_t x,y;
	bool touched = tft.getTouch( &x, &y, PRESSURE_THRESHOLD );

	if (touched)
	{
		g_press_x = x;
		g_press_y = y;
		g_pressed = true;
		g_debounce_time = now;
		// debug_serial("Touch(%d,%d)",touchX,touchY);
	}
	else
	{
		if (g_debounce_time &&
            now > g_debounce_time + DEBOUNCE_TIME)
        {
			g_debounce_time = 0;
        }
		if (!g_debounce_time)
		{
			g_pressed = false;

            // handle button up outside of hitTest
            // which does button down

            if (g_win_pressed)
            {
                const uiElement *was_pressed = g_win_pressed;
                g_win_pressed = NULL;
                // debug_serial("hitTest(%08x) onButton(false) %04x at (%d,%d,%d,%)",g_window_pressed,was_pressed->id_type,was_pressed->x,was_pressed->y,was_pressed->w,was_pressed->h);
                g_window_pressed->drawTypedElement(was_pressed,false);
                g_window_pressed->onButton(was_pressed,false);
                g_window_pressed = NULL;

            }
        }
	}
}



void getMutableElementSettings(
    const uiElement *ele,
    const char **text,
    uint16_t *bg,
    uint16_t *fg,
    FontType *font)
    // abstracted in case I wanna call it somewhere else
{
    if (ele->id_type & ID_TYPE_MUTABLE)
    {
        uiMutable *mut = (uiMutable *) ele->param;
        *text = mut->text;
        *bg   = mut->bg;
        *fg   = mut->fg;
        *font = mut->font;
    }
}


void uiWindow::drawTypedElement(const uiElement *ele, bool pressed)
{
    const char *text = (const char *) ele->param;
    uint16_t bg = ele->bg;
    uint16_t fg = ele->fg;
    FontType font = ele->font;
    getMutableElementSettings(ele,&text,&bg,&fg,&font);

    // if drawElement is called on the currently pressed button,
    // we force "pressed" to be true to prevent calls with false
    // (i.e. the percentage "main" button) from flashing ..

    if (ele == g_win_pressed)
        pressed = true;

    if (pressed)
    {
        bg = COLOR_BUTTON_BG_PRESSED;
        fg = COLOR_BUTTON_FG_PRESSED;
    }

    if (ele->id_type & ID_TYPE_BUTTON)
    {
        tft.fillRect(
            ele->x, ele->y, ele->w, ele->h,
            bg);
    }

    if (ele->id_type & ID_TYPE_TEXT)
    {
        drawText(
            text,
            ele->just,
            font,
            ele->x, ele->y, ele->w, ele->h,
            fg,
            bg );
    }
}



// standard begin() method draws all typed elements

void uiWindow::drawTypedElements()
{
    for (int i=0; i<m_num_elements; i++)
    {
        const uiElement *ele = &m_elements[i];
        if (ele->id_type && ID_TYPE_ALL)
        {
            drawTypedElement(ele,false);
        }
    }
}
