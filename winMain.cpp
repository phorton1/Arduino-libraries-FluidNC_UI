//--------------------------------------
// The 'main' idle/nav window
//--------------------------------------
// This window is used to show all states of the gMachine.
// It essentially varies from a general Navigation/UI when IDLE,
// and is modal to the BUSY, ALARM, HOMING, DOOR, ETC.
//
// It does this by switching it's underlying set of uiElements
// based on the JOB_MODE.
//
// It also changes the title of the "main" button, working in
// conjunction with gApplication to show the percent while busy,
// progress bar, etc.
//
// In fact there are only two different layouts for this window,
// one with the nav controls, and one with the reset-rebooot-etc
// buttons.



#include "winMain.h"
#include "dlgMsg.h"
#include "dlgConfirm.h"
#include "gPrefs.h"

#ifdef WITH_GRBL
    #include <System.h>
    #include <Serial.h>     // for CLIENTS and execute_realtime_command() and Cmd types
    #include <WebUI/InputBuffer.h>
#endif


winMain main_win;

//----------------------------------------------------------------------
// IDLE WINDOW DEFINITION
//----------------------------------------------------------------------

// #define IDX_HOME_BUTTON      0
// #define IDX_SET_BUTTON       1
#define IDX_MICRO_BUTTON        2
// #define IDX_X_MINUS2         3
// #define IDX_X_MINUS1         4
// #define IDX_X_PLUS1          5
// #define IDX_X_PLUS2          6
// #define IDX_Y_MINUS2         7
// #define IDX_Y_MINUS1         8
// #define IDX_Y_PLUS1          9
// #define IDX_Y_PLUS2          10
// #define IDX_Z_MINUS2         11
// #define IDX_Z_MINUS1         12
// #define IDX_Z_PLUS1          13
// #define IDX_Z_PLUS2          14
#define IDX_MACRO1              15
// #define IDX_MACRO2           16
// #define IDX_MACRO3           17
// #define IDX_MACRO4           18

#define NUM_IDLE_BUTTONS   19

#define ID_HOME_BUTTON1    ( 1 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
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

#define NUM_JOG_BUTTONS    12
#define NUM_MACRO_BUTTONS   4


static char idle_button_text[NUM_JOG_BUTTONS][6];
static char macro_button_text[NUM_MACRO_BUTTONS][2];


#define RC_TO_XY(r,c)      1+c*45 +(c==6?2:0), 35+r*34, 45,  35
    // 7 x 5     idle_buttons are 45 wide by 34 high
    // all idle_buttons are mutable (can be disabled)
    // later (with a preferences file) the macro names can change


static uiMutable idle_buttons[NUM_IDLE_BUTTONS] = {
    {"home",               COLOR_BLUE,           COLOR_WHITE, FONT_NORMAL },
    {"set",                COLOR_BLUE,           COLOR_WHITE, FONT_NORMAL },
    {"micro",              COLOR_BLUE,           COLOR_WHITE, FONT_NORMAL },
    {idle_button_text[ 0], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {idle_button_text[ 1], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {idle_button_text[ 2], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {idle_button_text[ 3], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {idle_button_text[ 4], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {idle_button_text[ 5], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {idle_button_text[ 6], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {idle_button_text[ 7], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {idle_button_text[ 8], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {idle_button_text[ 9], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {idle_button_text[10], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {idle_button_text[11], COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {macro_button_text[0], COLOR_BUTTON_HIDDEN,  COLOR_WHITE, FONT_BIG },
    {macro_button_text[1], COLOR_BUTTON_HIDDEN,  COLOR_WHITE, FONT_BIG },
    {macro_button_text[2], COLOR_BUTTON_HIDDEN,  COLOR_WHITE, FONT_BIG },
    {macro_button_text[3], COLOR_BUTTON_HIDDEN,  COLOR_WHITE, FONT_BIG },
};

static const uiElement idle_elements[] = {
    { ID_HOME_BUTTON1,    10,  52, 70, 34,  &idle_buttons[ 0], },
    { ID_SET_BUTTON  ,    10, 153, 70, 34,  &idle_buttons[ 1], },
    { ID_MICRO_BUTTON,   145,  52, 70, 34,  &idle_buttons[ 2], },
    { ID_X_MINUS2    ,   RC_TO_XY(2,0),     &idle_buttons[ 3], },
    { ID_X_MINUS1    ,   RC_TO_XY(2,1),     &idle_buttons[ 4], },
    { ID_X_PLUS1     ,   RC_TO_XY(2,3),     &idle_buttons[ 5], },
    { ID_X_PLUS2     ,   RC_TO_XY(2,4),     &idle_buttons[ 6], },
    { ID_Y_MINUS2    ,   RC_TO_XY(0,2),     &idle_buttons[ 7], },
    { ID_Y_MINUS1    ,   RC_TO_XY(1,2),     &idle_buttons[ 8], },
    { ID_Y_PLUS1     ,   RC_TO_XY(3,2),     &idle_buttons[ 9], },
    { ID_Y_PLUS2     ,   RC_TO_XY(4,2),     &idle_buttons[10], },
    { ID_Z_MINUS2    ,   RC_TO_XY(0,5),     &idle_buttons[11], },
    { ID_Z_MINUS1    ,   RC_TO_XY(1,5),     &idle_buttons[12], },
    { ID_Z_PLUS1     ,   RC_TO_XY(3,5),     &idle_buttons[13], },
    { ID_Z_PLUS2     ,   RC_TO_XY(4,5),     &idle_buttons[14], },
    { ID_MACRO1      ,   RC_TO_XY(0,6),     &idle_buttons[15], },
    { ID_MACRO2      ,   RC_TO_XY(1,6),     &idle_buttons[16], },
    { ID_MACRO3      ,   RC_TO_XY(2,6),     &idle_buttons[17], },
    { ID_MACRO4      ,   RC_TO_XY(3,6),     &idle_buttons[18], },
};

#define NUM_IDLE_ELEMENTS  (sizeof(idle_elements)/sizeof(uiElement))

//----------------------------------------------------------------------
// ACTIVE WINDOW DEFINITION
//----------------------------------------------------------------------

#define ID_ACTIVE_TEXT      (100 | ID_TYPE_TEXT | ID_TYPE_MUTABLE )
#define ID_HOME_BUTTON2     (101 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
#define ID_CPR_BUTTON       (102 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE )
#define ID_RESET_BUTTON     (103 | ID_TYPE_TEXT | ID_TYPE_BUTTON )
#define ID_REBOOT_BUTTON    (104 | ID_TYPE_TEXT | ID_TYPE_BUTTON )


static uiMutable active_text
{
    "",
    COLOR_BLACK,
    COLOR_YELLOW,
    FONT_BIG,
};
static uiMutable home_button = {
    "HOME",
    COLOR_BLUE,
    COLOR_WHITE,
    FONT_BIG,
};
static uiMutable cpr_button = {
    "",
    COLOR_BUTTON_DISABLED,
    COLOR_WHITE,
    FONT_BIG,
};


static const uiElement active_elements[] =
{
    { ID_ACTIVE_TEXT,     0,   45, 320,  30,  V(&active_text), },
    { ID_HOME_BUTTON2,    20,  95, 130,  40,  V(&home_button)  },
    { ID_CPR_BUTTON,     170,  95, 130,  40,  V(&cpr_button)   },
    { ID_RESET_BUTTON,    20, 145, 130,  40,  V("RESET"),      COLOR_ORANGE,  COLOR_BLACK,     FONT_BIG },
    { ID_REBOOT_BUTTON,  170, 145, 130,  40,  V("REBOOT"),     COLOR_DARKRED, COLOR_WHITE,     FONT_BIG },
};

#define NUM_ACTIVE_ELEMENTS  (sizeof(active_elements)/sizeof(uiElement))

const char *alarmText(uint8_t alarm)
{
    switch (alarm)
    {
        case 0  : return "Startup Alarm !!";
        case 1  : return "Hard Limit Alarm !!";
        case 2  : return "Soft Limit Alarm !!";
        case 3  : return "Abort Cycle Alarm !!";
        case 4  : return "Probe Fail Initial !!";
        case 5  : return "Probe Fail Contact !!";
        case 6  : return "Homing Fail Reset !!";
        case 7  : return "Homing Fail Door !!";
        case 8  : return "Homing Fail Pulloff !!";
        case 9  : return "Homing Fail Approach !!";
        case 10 : return "Spindle Control !!";
    }
    return "UNKNOWN_ALARM";
}


//----------------------------------
// implementation
//----------------------------------

winMain::winMain() :
    uiWindow(idle_elements,NUM_IDLE_ELEMENTS)
{}


void winMain::begin()
{
    m_draw_needed = 1;
}


void winMain::doJog(const char *axis, int jog_num)
    // G91=relative mode, must provide feed rate
{
    g_debug("doJog(%s%s F%d)",axis,idle_button_text[jog_num],getIntPref(PREF_JOG_FEED_RATE));

    #ifdef WITH_GRBL
        char command_buf[20];
        sprintf(command_buf,"$J=G91 %s%s F%d\r",
            axis,
            idle_button_text[jog_num],
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
                m_micro_mode = !m_micro_mode;
                break;
            case ID_HOME_BUTTON1 :
            case ID_HOME_BUTTON2 :
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

            // active mode buttons (ID_HOME_BUTTON2 handled above)

            case ID_RESET_BUTTON :
                confirm_dlg.setConfirm(CONFIRM_COMMAND_RESET);
                the_app.openWindow(&confirm_dlg);
                break;
            case ID_REBOOT_BUTTON :
                confirm_dlg.setConfirm(CONFIRM_COMMAND_REBOOT);
                the_app.openWindow(&confirm_dlg);
                break;
            case ID_CPR_BUTTON :
            {
                const char *b_text = ((uiMutable *) ele->param)->text;
                if (!strcmp(b_text,"CLEAR"))
                {
                    the_app.setTitle("");
                    #ifdef WITH_GRBL
                        WebUI::inputBuffer.push("$X\r");
                    #endif
                }
                else
                {
                    #ifdef WITH_GRBL
                        execute_realtime_command(
                            !strcmp(b_text,"RESUME") ?
                                Cmd::CycleStart : Cmd::FeedHold,
                            CLIENT_ALL);
                    #endif
                }
                break;
            }
        }
    }
}

void winMain::update()
{
    JobState job_state = the_app.getJobState();

    if (job_state != the_app.getLastJobState())
    {
        m_draw_needed = true;
        m_mode = job_state == JOB_IDLE ? MAIN_MODE_IDLE : MAIN_MODE_ACTIVE;

        g_debug("winMain setting m_mode to %d",m_mode);

        if (m_mode == MAIN_MODE_ACTIVE)
        {
            m_elements = active_elements;
            m_num_elements = NUM_ACTIVE_ELEMENTS;
        }
        else
        {
            m_elements = idle_elements;
            m_num_elements = NUM_IDLE_ELEMENTS;
        }
    }

    if (m_mode == MAIN_MODE_ACTIVE)
    {
        volatile uint8_t alarm = m_last_alarm;
            // we only grab the alarm when the job state has changed

        if (job_state != the_app.getLastJobState())
        {
            #ifdef WITH_GRBL
                alarm = static_cast<uint8_t>(sys_rt_exec_alarm);
                #if DEBUG_ALARM
                    g_debug("grabbed alarm=%d",alarm);
                #endif
            #endif
        }

        if (m_draw_needed ||
            m_last_alarm != alarm ||
            job_state != the_app.getLastJobState())
        {
            m_draw_needed = false;
            m_last_alarm = alarm;
            bool is_alarm = job_state == JOB_ALARM;

            home_button.bg = is_alarm ? COLOR_BLUE : COLOR_BUTTON_HIDDEN;

            active_text.text = is_alarm ?
                alarmText(alarm) :
                jobStateName(job_state);
            active_text.fg = is_alarm ?
                COLOR_MAGENTA :
                COLOR_YELLOW;

            if (job_state == JOB_ALARM)
            {
                char buf[12];
                sprintf(buf,"ALARM %d",alarm);
                the_app.setTitle(buf);

                cpr_button.bg = COLOR_BLUE;
                cpr_button.text = "CLEAR";
            }
            else if (job_state == JOB_BUSY ||
                     job_state == JOB_HOLD)
            {
                cpr_button.bg = job_state == JOB_BUSY ? COLOR_BLUE : COLOR_DARKGREEN;
                cpr_button.text = job_state == JOB_BUSY ? "PAUSE" : "RESUME";
            }
            else
            {
                cpr_button.bg = COLOR_BUTTON_HIDDEN;
                the_app.setTitle(jobStateName(job_state));
            }

            tft.fillRect(0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_CONTENT_HEIGHT,COLOR_BLACK);
            drawTypedElements();

        }   // changed
    }       // MAIN_MODE_ACTIVE
    else    // MAIN_MODE_IDLE
    {
        if (m_draw_needed ||
            m_last_micro_mode != m_micro_mode ||
            job_state != the_app.getLastJobState())
        {
            m_draw_needed = false;
            m_last_micro_mode = m_micro_mode;

            m_num_elements = NUM_IDLE_BUTTONS;

            idle_buttons[IDX_MICRO_BUTTON].bg = m_micro_mode ?
                COLOR_GREEN :
                COLOR_BLUE;
            idle_buttons[IDX_MICRO_BUTTON].fg = m_micro_mode ?
                COLOR_BLACK :
                COLOR_WHITE;

            const char *scale2 = m_micro_mode ? "1"    : "100";
            const char *scale1 = m_micro_mode ? "0.1"  : "10";
            const char *scale4 = m_micro_mode ? "1"    : "10";
            const char *scale3 = m_micro_mode ? "0.1"  : "1";

            sprintf(idle_button_text[ 0], "-%s",scale2);
            sprintf(idle_button_text[ 1], "-%s",scale1);
            sprintf(idle_button_text[ 2], "%s", scale1);
            sprintf(idle_button_text[ 3], "%s", scale2);
            sprintf(idle_button_text[ 4], "%s",scale2);
            sprintf(idle_button_text[ 5], "%s",scale1);
            sprintf(idle_button_text[ 6], "-%s", scale1);
            sprintf(idle_button_text[ 7], "-%s", scale2);
            sprintf(idle_button_text[ 8], "%s",scale4);
            sprintf(idle_button_text[ 9], "%s",scale3);
            sprintf(idle_button_text[10], "-%s", scale3);
            sprintf(idle_button_text[11], "-%s", scale4);

            strcpy(macro_button_text[0],getStrPref(PREF_MACRO1_CHAR));
            strcpy(macro_button_text[1],getStrPref(PREF_MACRO2_CHAR));
            strcpy(macro_button_text[2],getStrPref(PREF_MACRO3_CHAR));
            strcpy(macro_button_text[3],getStrPref(PREF_MACRO4_CHAR));

            idle_buttons[IDX_MACRO1].bg = COLOR_BLUE;

            tft.fillRect(0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_CONTENT_HEIGHT,COLOR_BLACK);
            drawTypedElements();

        }   // changed
    }       // MAIN_MODE_IDLE
}   // update()
