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
        uiWindow::draw();

        // TFT has no notion of alpha on colors or transparent backgrounds.
        // So every object must have an explicit background color that matches it's parent, and
        // if the parent bg changes, the child bg must be set somehow, for it to look right.

        tft.setTextColor(m_fg);   //,COLOR_YELLOW);    // m_bg);

        // setTextPadding() tells TFT to "fill the area behind the chars you draw with existing background color"
        // otherwise, TFT draws the text in the current text background color to the end of the screen
        // Note that TFT has no notion of our rectangle height.

        tft.setTextPadding(m_rect.w);

        // JUSTIFICATION
        // grrr - weird .. if we want to tell TFT to draw something centered we have to both tell
        // it that we want it centered, by setting the "datum" which is the origin point of the text to use,
        // and then specify the center of the rectangle ourselves, thus the "x,y" passed to drawString must
        // be coupled to the "datum".  what a lame api!
        //
        // NOTE that "top" and "bottom" depend on if the font has "ascenders" or "descenders" and
        // does not always line up "baseline" to zero (but at least doesn't overflow "bottom" or "top")

        uint8_t use_datum;
        int16_t use_x = m_rect.x;
        int16_t use_y = m_rect.y;

        switch (m_just)
        {
            case  JUST_LEFT:
                use_datum = ML_DATUM;
                break;
            case  JUST_RIGHT:
                use_datum = MR_DATUM;
                break;
            case  JUST_LEFT_TOP:
                use_datum = TL_DATUM;
                break;
            case  JUST_CENTER_TOP:
                use_datum = TC_DATUM;
                break;
            case  JUST_RIGHT_TOP:
                use_datum = TR_DATUM;
                break;
            case  JUST_LEFT_BOTTOM:
                use_datum = BL_DATUM;
                break;
            case  JUST_CENTER_BOTTOM:
                use_datum = BC_DATUM;
                break;
            case  JUST_RIGHT_BOTTOM:
                use_datum = BR_DATUM;
                break;
            default:
            case  JUST_CENTER:
                use_datum = MC_DATUM;
                break;
        }

        switch (m_just)
        {
            case  JUST_LEFT:
            case  JUST_LEFT_TOP:
            case  JUST_LEFT_BOTTOM:
                break;
            case  JUST_RIGHT:
            case  JUST_RIGHT_TOP:
            case  JUST_RIGHT_BOTTOM:
                use_x += m_rect.w;
                break;
            default:
            case  JUST_CENTER:
            case  JUST_CENTER_TOP:
            case  JUST_CENTER_BOTTOM:
                use_x += m_rect.w / 2;
                break;
        }

        switch (m_just)
        {
            case  JUST_LEFT_TOP:
            case  JUST_CENTER_TOP:
            case  JUST_RIGHT_TOP:
                break;
            case  JUST_LEFT_BOTTOM:
            case  JUST_CENTER_BOTTOM:
            case  JUST_RIGHT_BOTTOM:
                use_y += m_rect.h;
                break;
            default:
            case  JUST_LEFT:
            case  JUST_CENTER:
            case  JUST_RIGHT:
                use_y += m_rect.h / 2;
                break;
        }

        // FONTS - also poorly implemented with a bad API
        // SOLUTION:
        //
        //    #ifdef out any fonts included in TFT_eSPI but leave LOAD_GFXFF defined.
        //    The original GFX fonts will not be linked unless they are referenced.
        //    Created a new set of fonts in my fonts/ folder, at http://oleddisplay.squix.ch
        //    and added new ones that look good and make sense.
        //
        // TODO:
        //
        //    Implement TFT_eSPI text clipping rectangle.

        switch (m_font)
        {
            case FONT_TINY:
                tft.setFreeFont(&Dialog_plain_14);
                break;
            case FONT_SMALL:
                tft.setFreeFont(&Dialog_plain_16);
                break;
            case FONT_BIG:
                tft.setFreeFont(&Dialog_plain_22);
                break;
            case FONT_MONO:
                tft.setFreeFont(&Roboto_Mono_Bold_14);
                break;
            default:
            case FONT_NORMAL:
                tft.setFreeFont(&Dialog_plain_18);
                break;
        }

        // So now we can finally draw the text.

        tft.setTextDatum(use_datum);
        tft.drawString(m_text, use_x, use_y);

    }
}
