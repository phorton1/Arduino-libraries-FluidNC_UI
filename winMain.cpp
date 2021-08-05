//--------------------------------------
// The 'main' idle/nav window
//--------------------------------------

#include "winMain.h"

#include "dlgMsg.h"
#include "gPrefs.h"

#ifdef WITH_GRBL
    #include <System.h>
    #include <WebUI/InputBuffer.h>
#endif


winMain main_win;

//----------------------------------------------------------------------
// WINDOW DEFINITION
//----------------------------------------------------------------------

#define IDX_HOME_BUTTON     0
#define IDX_SET_BUTTON      1
#define IDX_MICRO_BUTTON    2
#define IDX_X_MINUS2        3
#define IDX_X_MINUS1        4
#define IDX_X_PLUS1         5
#define IDX_X_PLUS2         6
#define IDX_Y_MINUS2        7
#define IDX_Y_MINUS1        8
#define IDX_Y_PLUS1         9
#define IDX_Y_PLUS2        10
#define IDX_Z_MINUS2       11
#define IDX_Z_MINUS1       12
#define IDX_Z_PLUS1        13
#define IDX_Z_PLUS2        14
#define IDX_MACRO1         15
#define IDX_MACRO2         16
#define IDX_MACRO3         17
#define IDX_MACRO4         18

#define NUM_BUTTONS        19

#define ID_HOME_BUTTON     ( 1 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_SET_BUTTON      ( 2 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_MICRO_BUTTON    ( 3 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_X_MINUS2        ( 4 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_X_MINUS1        ( 5 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_X_PLUS1         ( 6 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_X_PLUS2         ( 7 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Y_MINUS2        ( 8 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Y_MINUS1        ( 9 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Y_PLUS1         (10 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Y_PLUS2         (11 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Z_MINUS2        (12 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Z_MINUS1        (13 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Z_PLUS1         (14 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Z_PLUS2         (15 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_MACRO1          (16 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_MACRO2          (17 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_MACRO3          (18 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_MACRO4          (19 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)

#define NUM_JOG_BUTTONS   12

static char button_text[NUM_JOG_BUTTONS][6];
static char macro_button_text[4][2];


#define RC_TO_XY(r,c)      1+c*45 +(c==6?2:0), 35+r*34, 45,  35
    // 7 x 5     buttons are 45 wide by 34 high
    // all buttons are mutable (can be disabled)
    // later (with a preferences file) the macro names can change


static uiMutable buttons[NUM_BUTTONS] = {
    {"home",               COLOR_BLUE,           COLOR_WHITE, FONT_NORMAL },
    {"set",                COLOR_BLUE,           COLOR_WHITE, FONT_NORMAL },
    {"micro",              COLOR_BLUE,           COLOR_WHITE, FONT_NORMAL },
    {button_text[ 0],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {button_text[ 1],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {button_text[ 2],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {button_text[ 3],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {button_text[ 4],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {button_text[ 5],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {button_text[ 6],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {button_text[ 7],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {button_text[ 8],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {button_text[ 9],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {button_text[10],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {button_text[11],      COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {macro_button_text[0], COLOR_BUTTON_HIDDEN,  COLOR_WHITE, FONT_BIG },
    {macro_button_text[1], COLOR_BUTTON_HIDDEN,  COLOR_WHITE, FONT_BIG },
    {macro_button_text[2], COLOR_BUTTON_HIDDEN,  COLOR_WHITE, FONT_BIG },
    {macro_button_text[3], COLOR_BUTTON_HIDDEN,  COLOR_WHITE, FONT_BIG },
};

static const uiElement idle_elements[] = {
    { ID_HOME_BUTTON ,    10,  52, 70, 34,  &buttons[ 0], },
    { ID_SET_BUTTON  ,    10, 153, 70, 34,  &buttons[ 1], },
    { ID_MICRO_BUTTON,   145,  52, 70, 34,  &buttons[ 2], },
    { ID_X_MINUS2    ,   RC_TO_XY(2,0),     &buttons[ 3], },
    { ID_X_MINUS1    ,   RC_TO_XY(2,1),     &buttons[ 4], },
    { ID_X_PLUS1     ,   RC_TO_XY(2,3),     &buttons[ 5], },
    { ID_X_PLUS2     ,   RC_TO_XY(2,4),     &buttons[ 6], },
    { ID_Y_MINUS2    ,   RC_TO_XY(0,2),     &buttons[ 7], },
    { ID_Y_MINUS1    ,   RC_TO_XY(1,2),     &buttons[ 8], },
    { ID_Y_PLUS1     ,   RC_TO_XY(3,2),     &buttons[ 9], },
    { ID_Y_PLUS2     ,   RC_TO_XY(4,2),     &buttons[10], },
    { ID_Z_MINUS2    ,   RC_TO_XY(0,5),     &buttons[11], },
    { ID_Z_MINUS1    ,   RC_TO_XY(1,5),     &buttons[12], },
    { ID_Z_PLUS1     ,   RC_TO_XY(3,5),     &buttons[13], },
    { ID_Z_PLUS2     ,   RC_TO_XY(4,5),     &buttons[14], },
    { ID_MACRO1      ,   RC_TO_XY(0,6),     &buttons[15], },
    { ID_MACRO2      ,   RC_TO_XY(1,6),     &buttons[16], },
    { ID_MACRO3      ,   RC_TO_XY(2,6),     &buttons[17], },
    { ID_MACRO4      ,   RC_TO_XY(3,6),     &buttons[18], },
};



//----------------------------------
// implementation
//----------------------------------

static bool g_draw_needed = false;
static bool g_micro_mode = false;
static bool g_last_micro_mode = false;


winMain::winMain() :
    uiWindow(idle_elements,(sizeof(idle_elements)/sizeof(uiElement)))
{}


void winMain::begin()
{
    g_draw_needed = 1;
}


void doJog(const char *axis, int jog_num)
    // G91=relative mode, must provide feed rate
{
    g_debug("doJog(%s%s F%d)",axis,button_text[jog_num],getIntPref(PREF_JOG_FEED_RATE));

    #ifdef WITH_GRBL
        char command_buf[20];
        sprintf(command_buf,"$J=G91 %s%s F%d\r",
            axis,
            button_text[jog_num],
            getIntPref(PREF_JOG_FEED_RATE));
        WebUI::inputBuffer.push(command_buf);
    #endif
}


void winMain::onButton(const uiElement *ele, bool pressed)
    // called before drawElement
{
    // g_debug("winMain::onButton(%04x) pressed=%d",ele->id_type,pressed);

    if (!pressed)
    {
        switch (ele->id_type)
        {
            case ID_MICRO_BUTTON:
                g_micro_mode = !g_micro_mode;
                break;
            case ID_HOME_BUTTON :
                the_app.setTitle("");
                #ifdef WITH_GRBL
                    WebUI::inputBuffer.push("$H\r");
                #endif
                break;
            case ID_X_MINUS2 :
            case ID_X_MINUS1 :
            case ID_X_PLUS1  :
            case ID_X_PLUS2  :
                 doJog("X",ele->id_type - ID_X_MINUS2);
                 break;
            case ID_Y_MINUS2 :
            case ID_Y_MINUS1 :
            case ID_Y_PLUS1  :
            case ID_Y_PLUS2  :
                doJog("Y",ele->id_type - ID_X_MINUS2);
                break;
            case ID_Z_MINUS2 :
            case ID_Z_MINUS1 :
            case ID_Z_PLUS1  :
            case ID_Z_PLUS2  :
                 doJog("Z",ele->id_type - ID_X_MINUS2);
                 break;
        }
    }
}


void winMain::update()
{
    JobState job_state = the_app.getJobState();

    if (g_draw_needed ||
        g_last_micro_mode != g_micro_mode ||
        job_state != the_app.getLastJobState())
    {
        g_draw_needed = false;
        g_last_micro_mode = g_micro_mode;

        tft.fillRect(0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_CONTENT_HEIGHT,COLOR_BLACK);

        if (job_state != JOB_IDLE)
        {
            drawText(
                jobStateName(job_state),
                JUST_CENTER,
                FONT_BIG,
                0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_CONTENT_HEIGHT,
                COLOR_YELLOW,
                COLOR_BLACK );
            m_num_elements = 0;
        }
        else
        {
            m_num_elements = NUM_BUTTONS;

            buttons[IDX_MICRO_BUTTON].bg = g_micro_mode ?
                COLOR_GREEN :
                COLOR_BLUE;
            buttons[IDX_MICRO_BUTTON].fg = g_micro_mode ?
                COLOR_BLACK :
                COLOR_WHITE;

            const char *scale2 = g_micro_mode ? "1"    : "100";
            const char *scale1 = g_micro_mode ? "0.1"  : "10";
            const char *scale4 = g_micro_mode ? "1"    : "10";
            const char *scale3 = g_micro_mode ? "0.1"  : "1";

            sprintf(button_text[ 0], "-%s",scale2);
            sprintf(button_text[ 1], "-%s",scale1);
            sprintf(button_text[ 2], "%s", scale1);
            sprintf(button_text[ 3], "%s", scale2);
            sprintf(button_text[ 4], "%s",scale2);
            sprintf(button_text[ 5], "%s",scale1);
            sprintf(button_text[ 6], "-%s", scale1);
            sprintf(button_text[ 7], "-%s", scale2);
            sprintf(button_text[ 8], "%s",scale4);
            sprintf(button_text[ 9], "%s",scale3);
            sprintf(button_text[10], "-%s", scale3);
            sprintf(button_text[11], "-%s", scale4);

            strcpy(macro_button_text[0],getStrPref(PREF_MACRO1_CHAR));
            strcpy(macro_button_text[1],getStrPref(PREF_MACRO2_CHAR));
            strcpy(macro_button_text[2],getStrPref(PREF_MACRO3_CHAR));
            strcpy(macro_button_text[3],getStrPref(PREF_MACRO4_CHAR));

            buttons[IDX_MACRO1].bg = COLOR_BLUE;


            drawTypedElements();

        }   // idle
    }   // changed
}   // update()
