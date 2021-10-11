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



#include "gPrefs.h"
#include "winMain.h"
#include "dlgMsg.h"
#include "dlgConfirm.h"
#include "dlgHome.h"
#include "dlgSetPosition.h"
#include "dlgFeedRates.h"


#include <gActions.h>    // FluidNC_extensions
#include <gStatus.h>     // FluidNC_extensions

#ifdef UI_WITH_MESH
    #include <Mesh.h>    // FluidNC_extensions
#endif


winMain main_win;

//----------------------------------------------------------------------
// IDLE WINDOW DEFINITION
//----------------------------------------------------------------------

// #define IDX_HOME_BUTTON      0
// #define IDX_SET_BUTTON       1
#define IDX_MICRO_BUTTON        2
// #define IDX_SYS_BUTTON       3
// #define IDX_X_MINUS2         4
// #define IDX_X_MINUS1         5
// #define IDX_X_PLUS1          6
// #define IDX_X_PLUS2          7
// #define IDX_Y_MINUS2         8
// #define IDX_Y_MINUS1         9
// #define IDX_Y_PLUS1          10
// #define IDX_Y_PLUS2          11
// #define IDX_Z_MINUS2         12
// #define IDX_Z_MINUS1         13
// #define IDX_Z_PLUS1          14
// #define IDX_Z_PLUS2          15
// #define IDZ_XY_ZERO          16
// #define IDX_Z_AZERO          17
// #define IDX_F_OVER           18
// #define IDX_S_OVER           19
// #define IDX_Z_OVER           20


#define ID_HOME_BUTTON1    ( 1 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_SET_BUTTON      ( 2 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_MICRO_BUTTON    ( 3 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_SYS_BUTTON      ( 4 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_X_MINUS2        ( 5 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_X_MINUS1        ( 6 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_X_PLUS1         ( 7 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_X_PLUS2         ( 8 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Y_PLUS2         ( 9 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Y_PLUS1         (10 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Y_MINUS1        (11 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Y_MINUS2        (12 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Z_PLUS2         (13 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Z_PLUS1         (14 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Z_MINUS1        (15 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Z_MINUS2        (16 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_XY_ZERO         (17 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_Z_ZERO          (18 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_F_OVER          (19 | ID_TYPE_TEXT | ID_TYPE_BUTTON )
#define ID_S_OVER          (20 | ID_TYPE_TEXT | ID_TYPE_BUTTON )
#define ID_Z_OVER          (21 | ID_TYPE_TEXT | ID_TYPE_BUTTON )


#define NUM_JOG_BUTTONS    12

#ifdef UI_WITH_MESH
    #define NUM_OVER_BUTTONS    3
#else
    #define NUM_OVER_BUTTONS    2
#endif

static char jog_button_text[NUM_JOG_BUTTONS][6];
static char feed_over_text[8];
static char spindle_over_text[8];
#ifdef UI_WITH_MESH
    static char z_over_text[8];
#endif


#define RC_TO_XY(r,c)      1+c*45 +(c==6?2:0), 35+r*34, 45,  35
    // 7 x 5     idle_buttons are 45 wide by 34 high

// these don't all need to be mutable

static uiMutable idle_buttons[] = {
    {"home",               COLOR_BLUE,           COLOR_WHITE, FONT_NORMAL },
    {"set",                COLOR_BLUE,           COLOR_WHITE, FONT_NORMAL },
    {"micro",              COLOR_BLUE,           COLOR_WHITE, FONT_NORMAL },
    {"sys",                COLOR_BLUE,           COLOR_WHITE, FONT_NORMAL },
    {jog_button_text[ 0],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {jog_button_text[ 1],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {jog_button_text[ 2],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {jog_button_text[ 3],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {jog_button_text[ 4],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {jog_button_text[ 5],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {jog_button_text[ 6],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {jog_button_text[ 7],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {jog_button_text[ 8],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {jog_button_text[ 9],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {jog_button_text[10],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {jog_button_text[11],  COLOR_DARKGREEN,      COLOR_WHITE, FONT_MONO },
    {"0",                  COLOR_BLUE,           COLOR_WHITE, FONT_NORMAL },
    {"safe",               COLOR_BLUE,           COLOR_WHITE, FONT_MONO },
};

static const uiElement idle_elements[] = {
    { ID_HOME_BUTTON1,    10,  52, 70, 34,  &idle_buttons[ 0], },
    { ID_SET_BUTTON  ,    10, 153, 70, 34,  &idle_buttons[ 1], },
    { ID_MICRO_BUTTON,   145,  52, 70, 34,  &idle_buttons[ 2], },
    { ID_SYS_BUTTON  ,   145, 153, 70, 34,  &idle_buttons[ 3], },
    { ID_X_MINUS2    ,   RC_TO_XY(2,0),     &idle_buttons[ 4], },
    { ID_X_MINUS1    ,   RC_TO_XY(2,1),     &idle_buttons[ 5], },
    { ID_X_PLUS1     ,   RC_TO_XY(2,3),     &idle_buttons[ 6], },
    { ID_X_PLUS2     ,   RC_TO_XY(2,4),     &idle_buttons[ 7], },
    { ID_Y_PLUS2     ,   RC_TO_XY(0,2),     &idle_buttons[ 8], },
    { ID_Y_PLUS1     ,   RC_TO_XY(1,2),     &idle_buttons[ 9], },
    { ID_Y_MINUS1    ,   RC_TO_XY(3,2),     &idle_buttons[10], },
    { ID_Y_MINUS2    ,   RC_TO_XY(4,2),     &idle_buttons[11], },
    { ID_Z_PLUS2     ,   RC_TO_XY(0,5),     &idle_buttons[12], },
    { ID_Z_PLUS1     ,   RC_TO_XY(1,5),     &idle_buttons[13], },
    { ID_Z_MINUS1    ,   RC_TO_XY(3,5),     &idle_buttons[14], },
    { ID_Z_MINUS2    ,   RC_TO_XY(4,5),     &idle_buttons[15], },
    { ID_XY_ZERO     ,   RC_TO_XY(2,2),     &idle_buttons[16], },
    { ID_Z_ZERO      ,   RC_TO_XY(2,5),     &idle_buttons[17], },
    { ID_F_OVER      ,   RC_TO_XY(0,6),     &feed_over_text,    COLOR_BLUE, COLOR_WHITE, FONT_MONO },
    { ID_S_OVER      ,   RC_TO_XY(1,6),     &spindle_over_text, COLOR_BLUE, COLOR_WHITE, FONT_MONO },
#ifdef UI_WITH_MESH
    { ID_Z_OVER      ,   RC_TO_XY(4,6),     &z_over_text,       COLOR_BLUE, COLOR_WHITE, FONT_MONO },
#endif
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
#define ID_F_OVER2          (105 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE | ID_TYPE_DONT_DRAW_HIDDEN )
#define ID_S_OVER2          (106 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE | ID_TYPE_DONT_DRAW_HIDDEN )
#define ID_Z_OVER2          (107 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE | ID_TYPE_DONT_DRAW_HIDDEN )

static char active_text_buffer[30];

static uiMutable active_text
{
    "",
    COLOR_BLACK,
    COLOR_YELLOW,
    FONT_NORMAL,
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
static uiMutable feed_over_button = {
    feed_over_text,
    COLOR_BUTTON_HIDDEN,
    COLOR_WHITE,
    FONT_SMALL,
};
static uiMutable spindle_over_button = {
    spindle_over_text,
    COLOR_BUTTON_HIDDEN,
    COLOR_WHITE,
    FONT_SMALL,
};
#ifdef UI_WITH_MESH
    static uiMutable z_over_button = {
        z_over_text,
        COLOR_BUTTON_HIDDEN,
        COLOR_WHITE,
        FONT_SMALL,
    };
#endif


static const uiElement active_elements[] =
{
    { ID_ACTIVE_TEXT,     0,   45, 320,  30,  V(&active_text), },
    { ID_HOME_BUTTON2,    20,  95, 130,  40,  V(&home_button)  },
    { ID_F_OVER2,         20,  80,  60,  33,  V(&feed_over_button),    },
    { ID_S_OVER2,         20, 115,  60,  33,  V(&spindle_over_button), },
#ifdef UI_WITH_MESH
    { ID_Z_OVER2,         90,  80,  60,  33,  V(&z_over_button),       },
#endif
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
    m_last_job_state = JOB_NONE;
    m_last_feed_override = 0;
    m_last_spindle_override  = 0;
    #ifdef UI_WITH_MESH
        m_last_live_z = 0;
    #endif
}


void winMain::doJog(const char *axis, int jog_num)
    // G91=relative mode, must provide feed rate
{
    g_debug("doJog(%s%s F%d)",axis,jog_button_text[jog_num],getIntPref(PREF_JOG_FEED_RATE));
    char command_buf[20];
    sprintf(command_buf,"$J=G91 %s%s F%d\r",
        axis,
        jog_button_text[jog_num],
        getIntPref(PREF_JOG_FEED_RATE));
    gActions::pushGrblText(command_buf);
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
            case ID_SET_BUTTON:
                the_app.setTitle("");
                the_app.openWindow(&dlg_position);
                break;
            case ID_HOME_BUTTON1 :
            case ID_HOME_BUTTON2 :
                the_app.setTitle("");
                the_app.openWindow(&dlg_home);
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
            case ID_XY_ZERO :
                // Programmed Zero
                gActions::pushGrblText("G0 X0 Y0\r");
                break;
            case ID_Z_ZERO :
                {
                    // Absolute 0, actually it's minus the z_axis pulloff
                    char buf[30];
                    sprintf(buf,"G0 G53 Z%5.3f\r", - gStatus::getAxisPulloff(UI_AXIS_Z) );
                    g_debug("AZERO(%s)",buf);
                    gActions::pushGrblText(buf);
                    break;
                }
            case ID_F_OVER2 :
            case ID_F_OVER :
                dlg_feed_rates.setSelectedItem(PARAM_IDX_FEED_RATE);
                the_app.openWindow(&dlg_feed_rates);
                break;
            case ID_S_OVER2 :
            case ID_S_OVER :
                dlg_feed_rates.setSelectedItem(PARAM_IDX_SPINDLE_RATE);
                the_app.openWindow(&dlg_feed_rates);
                break;
            #ifdef UI_WITH_MESH
                case ID_Z_OVER2 :
                case ID_Z_OVER :
                    dlg_feed_rates.setSelectedItem(PARAM_IDX_LIVE_Z);
                    the_app.openWindow(&dlg_feed_rates);
                    break;
            #endif

            // active mode buttons (ID_HOME_BUTTON2 handled above)

            case ID_RESET_BUTTON :
                dlg_confirm.setConfirm(CONFIRM_COMMAND_RESET);
                the_app.openWindow(&dlg_confirm);
                break;
            case ID_REBOOT_BUTTON :
                dlg_confirm.setConfirm(CONFIRM_COMMAND_REBOOT);
                the_app.openWindow(&dlg_confirm);
                break;
            case ID_CPR_BUTTON :
            {
                const char *b_text = ((uiMutable *) ele->param)->text;
                if (!strcmp(b_text,"CLEAR"))
                {
                    the_app.setTitle("");
                    gActions::pushGrblText("$X\r");
                }
                else
                {
                    gActions::realtime_command(
                        !strcmp(b_text,"RESUME") ? Cmd::CycleStart : Cmd::FeedHold);
                }
                break;
            }
        }
    }
}


void winMain::setLastOverrideText()
{
    m_last_feed_override = g_status.getFeedOverride();
    m_last_spindle_override = g_status.getSpindleOverride();
    sprintf(feed_over_text,"F%d",m_last_feed_override);
    sprintf(spindle_over_text,"S%d",m_last_spindle_override);
    #ifdef UI_WITH_MESH
        m_last_live_z = the_mesh.getLiveZ();
        sprintf(z_over_text,"%0.3f",m_last_live_z);
        int p = 0;
        if (z_over_text[p] == '-') p++;
        if (z_over_text[p] == '0')
            strcpy(&z_over_text[p],&z_over_text[p+2]);
    #endif
}


void winMain::update()
{
    JobState job_state = the_app.getJobState();

    if (m_last_job_state != job_state)
    {
        m_draw_needed = true;
        m_last_job_state = job_state;
        m_mode = job_state == JOB_IDLE ? MAIN_MODE_IDLE : MAIN_MODE_ACTIVE;

        g_debug("winMain setting m_mode to %d",m_mode);

        if (m_last_mode != m_mode)
        {
            m_last_mode = m_mode;
            the_app.setTitle("");

            // a weird place to reset m_doing_probe,
            // but we have to wait until the probe
            // is good and done

            if (m_mode == 0 && dlg_home.m_doing_probe)
            {
                // if a probe (from UI) has been completed,
                // set the Z zero position
                if (gActions::getProbeSucceeded())
                {
                    dlg_home.m_doing_probe = false;
                    g_debug("PROBE COMPLETED");
                    vTaskDelay(500);
                    gActions::pushGrblText("G10 L20 Z0\r");
                    vTaskDelay(1000);
                    gActions::pushGrblText("G0 Z5\r");
                    vTaskDelay(500);
                }
                else
                {
                    dlg_home.m_doing_probe = false;
                    g_debug("PROBE FAILED");
                }
            }
        }

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
        uint8_t alarm = g_status.getLastAlaram();
            // gStatus only grabs the alarm when the job state has changed

        #ifdef UI_WITH_MESH
            static int last_mesh_num_steps = 0;
            static int last_mesh_step = 0;
        #endif

        if (m_draw_needed ||
            m_last_alarm != alarm ||
            job_state != the_app.getLastJobState() ||
            m_last_feed_override != g_status.getFeedOverride() ||
            m_last_spindle_override != g_status.getSpindleOverride()
            #ifdef UI_WITH_MESH
                || m_last_live_z != the_mesh.getLiveZ()
                || last_mesh_num_steps != the_mesh.getNumSteps()
                || last_mesh_step != the_mesh.getCurStep()
            #endif
            )
        {
            m_draw_needed = false;
            m_last_alarm = alarm;

            setLastOverrideText();

            #ifdef UI_WITH_MESH
                last_mesh_num_steps = the_mesh.getNumSteps();
                last_mesh_step = the_mesh.getCurStep();
            #endif

            bool is_alarm = job_state == JOB_ALARM;
            bool in_job = job_state == JOB_BUSY || job_state == JOB_HOLD;

            home_button.bg = is_alarm ? COLOR_BLUE : COLOR_BUTTON_HIDDEN;
            feed_over_button.bg = in_job ? COLOR_BLUE : COLOR_BUTTON_HIDDEN;
            spindle_over_button.bg = in_job ? COLOR_BLUE : COLOR_BUTTON_HIDDEN;

            #ifdef UI_WITH_MESH
                z_over_button.bg = in_job ? COLOR_BLUE : COLOR_BUTTON_HIDDEN;
            #endif

            active_text.text =
                is_alarm ? alarmText(alarm) :
                in_job   ? the_app.getActiveFilename() :
                jobStateName(job_state);
            active_text.fg =
                is_alarm ? COLOR_MAGENTA :
                job_state == JOB_HOLD ? COLOR_CYAN :
                COLOR_YELLOW;

            #ifdef UI_WITH_MESH
                if (job_state == JOB_MESHING)
                {
                    char buf[20];
                    sprintf(buf,"MESH %d/%d",last_mesh_step,last_mesh_num_steps);
                    the_app.setTitle(buf);

                    sprintf(active_text_buffer,"MESH %d%% complete",
                        (100 * last_mesh_step) / last_mesh_num_steps);
                    active_text.text = active_text_buffer;
                }
                else
            #endif

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
                the_app.setTitle(jobStateName(job_state));
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
        uint8_t wifi_state = g_status.getWifiState();
        if (m_draw_needed ||
            m_last_wifi_state != wifi_state ||
            m_last_micro_mode != m_micro_mode ||
            job_state != the_app.getLastJobState() ||
            m_last_feed_override != g_status.getFeedOverride() ||
            m_last_spindle_override != g_status.getSpindleOverride()
            #ifdef UI_WITH_MESH
                || m_last_live_z != the_mesh.getLiveZ()
            #endif
            )
        {
            m_draw_needed = false;
            m_last_micro_mode = m_micro_mode;
            m_last_wifi_state = wifi_state;

            setLastOverrideText();

            idle_buttons[IDX_MICRO_BUTTON].bg = m_micro_mode ?
                COLOR_GREEN :
                COLOR_BLUE;
            idle_buttons[IDX_MICRO_BUTTON].fg = m_micro_mode ?
                COLOR_BLACK :
                COLOR_WHITE;

            // JOG Buttons

            const char *xy_scale1 = m_micro_mode ? "0.1"  : "10";
            const char *xy_scale2 = m_micro_mode ? "1"    : "100";

            const char *z_scale1 = m_micro_mode ? "0.01"  : "1";
            const char *z_scale2 = m_micro_mode ? "0.10"  : "10";

            sprintf(jog_button_text[ 0], "-%s", xy_scale2);      // X_MINUS2
            sprintf(jog_button_text[ 1], "-%s", xy_scale1);      // X_MINUS1
            sprintf(jog_button_text[ 2], "%s",  xy_scale1);      // X_PLUS1
            sprintf(jog_button_text[ 3], "%s",  xy_scale2);      // X_PLUS2

            sprintf(jog_button_text[ 4], "%s",  xy_scale2);      // Y_PLUS2
            sprintf(jog_button_text[ 5], "%s",  xy_scale1);      // Y_PLUS1
            sprintf(jog_button_text[ 6], "-%s", xy_scale1);      // Y_MINUS1
            sprintf(jog_button_text[ 7], "-%s", xy_scale2);      // Y_MINUS2

            sprintf(jog_button_text[ 8], "%s",  z_scale2);       // Z_PLUS2
            sprintf(jog_button_text[ 9], "%s",  z_scale1);       // Z_PLUS1
            sprintf(jog_button_text[10], "-%s", z_scale1);       // Z_MINUS1`
            sprintf(jog_button_text[11], "-%s", z_scale2);       // Z_MINUS2

            tft.fillRect(0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_CONTENT_HEIGHT,COLOR_BLACK);
            drawTypedElements();

            if (wifi_state == IND_STATE_READY ||
                wifi_state == IND_STATE_ACTIVE)
            {
                the_app.setTitle(gStatus::getIPAddress());
            }

        }   // changed
    }   // MAIN_MODE_IDLE
}   // update()
