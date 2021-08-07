//--------------------------------------
// The main Alarm window
//--------------------------------------

#include "uiScrollable.h"
#include "gBuffer.h"


//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------


#define NUM_BUTTONS         4

#define IDX_BACK_BUTTON     0
#define IDX_UP_BUTTON       1
#define IDX_DOWN_BUTTON     2
#define IDX_ROOT_BUTTON     3

#define ID_BACK_BUTTON   (0x0001 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
#define ID_UP_BUTTON     (0x0002 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
#define ID_DOWN_BUTTON   (0x0003 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
#define ID_ROOT_BUTTON   (0x0004 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )

#define ID_LINE1     (0x0011 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_DUAL)
#define ID_LINE2     (0x0012 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_DUAL)
#define ID_LINE3     (0x0013 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_DUAL)
#define ID_LINE4     (0x0014 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_DUAL)
#define ID_LINE5     (0x0015 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_DUAL)

static uiMutable buttons[NUM_BUTTONS] =
{
    { "..", COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
    { "^",  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
    { "v",  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
    { "/",  COLOR_BUTTON_HIDDEN, COLOR_WHITE, FONT_BIG },
};

static uiDualElement lines[MAX_SCROLLABLE_LINES] =
{
    { ELEMENT_TYPE_STR, COLOR_BUTTON_HIDDEN, 190, 70, NULL, NULL, FONT_MONO },
    { ELEMENT_TYPE_STR, COLOR_BUTTON_HIDDEN, 190, 70, NULL, NULL, FONT_MONO },
    { ELEMENT_TYPE_STR, COLOR_BUTTON_HIDDEN, 190, 70, NULL, NULL, FONT_MONO },
    { ELEMENT_TYPE_STR, COLOR_BUTTON_HIDDEN, 190, 70, NULL, NULL, FONT_MONO },
    { ELEMENT_TYPE_STR, COLOR_BUTTON_HIDDEN, 190, 70, NULL, NULL, FONT_MONO },
};


static const uiElement scroll_elements[] =
{
    { ID_BACK_BUTTON, 270,  37, 48, 42, V(&buttons[0]) },
    { ID_UP_BUTTON  , 270,  79, 48, 42, V(&buttons[1]) },
    { ID_DOWN_BUTTON, 270, 121, 48, 42, V(&buttons[2]) },
    { ID_ROOT_BUTTON, 270, 163, 48, 42, V(&buttons[3]) },

    { ID_LINE1, 0,   35, 265, 34, V(&lines[0]), COLOR_BLUE,  COLOR_WHITE, FONT_MONO, JUST_LEFT },
    { ID_LINE2, 0,   69, 265, 34, V(&lines[1]), COLOR_BLUE,  COLOR_WHITE, FONT_MONO, JUST_LEFT },
    { ID_LINE3, 0,  103, 265, 34, V(&lines[2]), COLOR_BLUE,  COLOR_WHITE, FONT_MONO, JUST_LEFT },
    { ID_LINE4, 0,  137, 265, 34, V(&lines[3]), COLOR_BLUE,  COLOR_WHITE, FONT_MONO, JUST_LEFT },
    { ID_LINE5, 0,  171, 265, 34, V(&lines[4]), COLOR_BLUE,  COLOR_WHITE, FONT_MONO, JUST_LEFT },
};




//----------------------
// implementation
//----------------------

uiScrollable::uiScrollable() :
    uiWindow(scroll_elements,sizeof(scroll_elements)/sizeof(uiElement))
{}


void uiScrollable::begin()
{
    m_draw_needed = true;
    tft.fillRect(0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_CONTENT_HEIGHT,COLOR_BLACK);
}



void uiScrollable::onButton(const uiElement *ele, bool pressed)
    // called before drawElement
{
    if (!pressed)
    {
        switch (ele->id_type)
        {
            case ID_BACK_BUTTON :
                onGoBack();
                break;
            case ID_UP_BUTTON :
                m_top_num--;
                break;
            case ID_DOWN_BUTTON :
                m_top_num++;
                break;
            case ID_ROOT_BUTTON :
                onGoHome();
                break;
            case ID_LINE1 :
            case ID_LINE2 :
            case ID_LINE3 :
            case ID_LINE4 :
            case ID_LINE5 :
                {
                    int rel_num = ele->id_type-ID_LINE1;
                    onLineClicked(&lines[rel_num],m_top_num,rel_num);
                    break;
                }
        }
    }
}



void uiScrollable::update()
{
    if (m_buffer_version != gs_buffer_version)
    {
        m_draw_needed = true;
        m_buffer_version = updateBuffer();

        // on updateBuffer(), constrain top_num

        if (m_top_num > m_num_lines - MAX_SCROLLABLE_LINES)
        {
            m_top_num = m_num_lines - MAX_SCROLLABLE_LINES;
            if (m_top_num < 0) m_top_num = 0;
        }
    }
    if (m_draw_needed ||
        m_last_top_num != m_top_num ||
        m_last_err_msg != m_err_msg)
    {
        if (m_last_err_msg != m_err_msg)
            tft.fillRect(0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_CONTENT_HEIGHT,COLOR_BLACK);
        m_draw_needed = false;
        m_last_top_num = m_top_num;
        m_last_err_msg = m_err_msg;

        the_app.setTitle(getTitle());

        if (m_err_msg)
        {
            drawText(
                m_err_msg,
                JUST_CENTER,
                FONT_BIG,
                0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_CONTENT_HEIGHT,
                COLOR_RED,
                COLOR_BLACK);
        }
        else
        {
            for (int i=0; i<NUM_BUTTONS; i++)
                buttons[i].bg = COLOR_BUTTON_HIDDEN;
            for (int i=0; i<MAX_SCROLLABLE_LINES; i++)
                lines[i].bg = COLOR_BUTTON_HIDDEN;

            int rel_line = 0;
            int abs_line = m_top_num;
            while (abs_line < m_num_lines &&
                   rel_line < MAX_SCROLLABLE_LINES)
            {
                uiDualElement *dual = &lines[rel_line];
                dual->bg = COLOR_DARKBLUE;
                getLine(abs_line,&dual->label,(const char **)&dual->value);
                rel_line++;
                abs_line++;
            }

            if (canGoBack())
                buttons[IDX_BACK_BUTTON].bg = COLOR_BLUE;
            if (m_top_num)
                buttons[IDX_UP_BUTTON].bg = COLOR_BLUE;
            if (m_top_num + MAX_SCROLLABLE_LINES < m_num_lines)
                buttons[IDX_DOWN_BUTTON].bg = COLOR_BLUE;
            if (canGoHome())
                buttons[IDX_ROOT_BUTTON].bg = COLOR_BLUE;

            drawTypedElements();
        }
    }
}
