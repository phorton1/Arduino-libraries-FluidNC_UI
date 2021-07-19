//--------------------------------------
// GRBL application specific classes
//--------------------------------------
// gApplication, gTitleBar, gContentArea, and gStatusBar


#include "gApp.h"
#include "gStatus.h"
#include "Grbl_MinUI.h"
#include "myTFT.h"


//  enum jobState_t
//  {
//      JOB_STATE_NONE = 0,
//      JOB_STATE_STARTED,
//      JOB_STATE_PAUSED,
//      JOB_STATE_COMPLETE,
//      JOB_STATE_ABORTED,
//      JOB_STATE_ERROR
//  };



#define MAX_TITLE  30
#define MAX_BUTTON 7


// IDS OF ANY PARENTS MUST BE GLOBALLY UNIQUE !!

#define ID_NONE                 0
#define ID_APP                  50
#define ID_TITLEBAR             100
#define ID_APP_BUTTON           101
#define ID_APP_TITLE            102
#define ID_SD_INDICATOR         103
#define ID_WIFI_INDICATOR       104


#define ID_STATUSBAR            200
#define ID_MACHINE_X            201
#define ID_MACHINE_Y            202
#define ID_MACHINE_Z            203
#define ID_WORK_X               204
#define ID_WORK_Y               205
#define ID_WORK_Z               206
#define ID_GRBL_STATE           207
#define ID_MEMAVAIL             208
#define ID_MEMAVAIL_MIN          209


#define ID_STEXT1               201
#define ID_STEXT2               203
#define ID_BUTTON1              301
#define ID_BUTTON2              302
#define ID_BUTTON3              303


class uiWin;    // forward

typedef int (*intFunction)();
typedef void (*winCallback)(const uiWin*) ;


class uiWin
{
public:
    const int16_t id;
    const int16_t parent_id;
    const int16_t x;
    const int16_t y;
    const int16_t w;
    const int16_t h;
    const winCallback fn_cb;
    const int16_t param;
    const int16_t bg;
    const int16_t fg;
    const FontType font;
    const JustifyType just;
};


//----------------------
// vars
//----------------------

static struct frameLast_t
    // we keep a separate SD state for the main_button and title
{
    char           app_title[MAX_TITLE + 1];
    uint8_t        wifi_state;
    grbl_State_t   sys_state;
    grbl_SDState_t sd_state;
    // jobState_t     job_state;
    float          pct;
    float          pos[UI_NUM_AXES * 2];
    uint32_t       mem_avail;
    uint32_t       min_avail;

    int16_t        prog_w;
    uint16_t       prog_color;
} frame_last;



static bool app_inited = false;
static char app_title[MAX_TITLE + 1] = "app title";
static char app_button[MAX_BUTTON + 1] = "MAIN";

static grbl_SDState_t sd_state;
static grbl_State_t sys_state;
// static jobState_t job_state = JOB_STATE_NONE;

int16_t  prog_x = 0;
int16_t  prog_w = 0;
uint16_t prog_color = 0;



//------------------------
// utilities
//------------------------
//
//  const char *jobStateName(jobState_t state)
//  {
//  	switch (state)
//  	{
//  		case JOB_STATE_NONE		: return "NONE";
//  		case JOB_STATE_STARTED	: return "STARTED";
//  		case JOB_STATE_COMPLETE : return "COMPLETE";
//  		case JOB_STATE_ABORTED  : return "ABORTED";
//  		case JOB_STATE_ERROR	: return "ERROR";
//  	}
//  	return "UNKNOWN_JOB";
//  }


uint16_t indColor(uint8_t ind_state)
{
    return
        ind_state == IND_STATE_ENABLED ? COLOR_CYAN :
        ind_state == IND_STATE_READY   ? COLOR_GREEN :
        ind_state == IND_STATE_ACTIVE  ? COLOR_YELLOW :
        ind_state == IND_STATE_ERROR   ? COLOR_RED :
        COLOR_LIGHTGREY;
}


void doText(const uiWin *win, const char *text)
{
    drawText(
        text,
        win->just,
        win->font,
        win->x, win->y, win->w, win->h,
        win->fg,
        win->bg );
}


void doTextProgress(const uiWin *win, const char *text, bool changed)
    // Called for fields that lie on the progress bar to draw the text.
    // Called if the field has changed or frame_last.prog_w != prog_w,
    // which implies that a new bar portion has been drawn.
{
    // develop s,e notation

    int16_t win_s = win->x;
    int16_t win_e = win->x + win->w - 1;
    int16_t prog_s = prog_x + frame_last.prog_w;
    int16_t prog_e = prog_x + prog_w - 1;

    // win_s > prog_e implies that there is no progress bar or
    // the field lays completely to the right of the progress bar
    // so we only write the value if it changed using default background

    if (win_s > prog_e)
    {
        if (changed)
            doText(win,text);
        return;
    }

    // At least part of the field overlays the progress bar.
    // We will redraw if it has changed, or if the field
    // is over the "new" bar portion.

    // If it was changed, and the field is to the left
    // of the progress bar, we stretch prog_s to the beginning
    // of the field to draw fully 'changed' bar backgrounds

    if (changed && prog_s > win_s)
    {
        prog_s = win_s;
    }

    // Constrain the new progress area to the field window.
    // bar will always start from the left.

    if (prog_s < win_s)
        prog_s = win_s;
    if (prog_e > win_e)
        prog_e = win_e;

    // So now we can just compare and see if the window
    // falls into the extended region, and leave if not.

    if (!changed && (win_e < prog_s || win_s > prog_e))
    {
        return;
    }

    // Calculate the "blue" portion if any

    int16_t normal_s = win_s;
    int16_t normal_w = 0;
    if (prog_e < win_e)
    {
        normal_s = prog_e + 1;
        normal_w = win_e - prog_e;
    }

    // Change bsrt back to x,w format and
    // draw the bar and normal bg portions, if any.

    int16_t bar_w = prog_e - prog_s + 1;

    if (bar_w > 0)
        tft.fillRect(prog_s,win->y,bar_w,win->h,prog_color);
    if (normal_w > 0)
        tft.fillRect(normal_s,win->y,normal_w,win->h,win->bg);

    // Finally ... draw the text with no background color

    drawText(
        text,
        win->just,
        win->font,
        win->x, win->y, win->w, win->h,
        win->fg,
        win->fg );      // bg <= fg implies no background
}


//--------------------------------
// frame callbacks
//--------------------------------

void doWin(const uiWin *win)
{
    if (!app_inited)
    {
        tft.fillRect(
            win->x, win->y, win->w, win->h,
            win->bg);
    }
}


void doAppButton(const uiWin *win)
{
    if (!app_inited)
    {
        tft.fillRect(
            win->x, win->y, win->w, win->h,
            COLOR_BLUE);
    }

    float pct = g_status.filePct();

     if (!app_inited ||
         frame_last.sd_state != sd_state ||
         frame_last.sys_state != sys_state ||
         frame_last.pct != pct)
    {
        if (sd_state == grbl_SDState_t::Busy)
        {
            char buf[12];
            sprintf(buf,"%2.1f%%",pct);
            uint16_t color = sys_state == grbl_State_t::Hold ?
                COLOR_CYAN : COLOR_YELLOW;

            drawText(
                buf,
                win->just,
                FONT_NORMAL,
                win->x, win->y, win->w, win->h,
                color,
                win->bg );
        }
        else
            doText(win,app_button);
    }
}


void doAppTitle(const uiWin *win)
{
    if (!app_inited ||
        frame_last.sd_state != sd_state ||
        frame_last.sys_state != sys_state)
    {
        uint16_t color = win->fg;

        // this is where we start a "Job"

        if (frame_last.sd_state != sd_state &&
            sd_state == grbl_SDState_t::Busy)
        {
            const char *filename = g_status.getActiveFilename();
            const char *p = filename;
            while (*p)
            {
                if (*p++ == '/')
                filename = p;
            }
            strncpy(app_title, filename, MAX_TITLE);
            app_title[MAX_TITLE] = 0;
        }

        // and this is where we  potentially change from "Busy" to "Hold"

        if (sd_state == grbl_SDState_t::Busy)
        {
            color = sys_state == grbl_State_t::Hold ?
                COLOR_CYAN : COLOR_YELLOW;
            prog_color = sys_state == grbl_State_t::Hold ?
                COLOR_DARKCYAN : COLOR_BROWN;

            // invalidate the progress bar upon a color change

            if (frame_last.prog_color != prog_color)
            {
                frame_last.prog_w = 0;
            }
        }
        else
        {
            strcpy(app_title,"NO JOB");
        }

        drawText(
            app_title,
            win->just,
            win->font,
            win->x, win->y, win->w, win->h,
            color,
            win->bg );
    }
}


void doSDIndicator(const uiWin *win)
{
    if (!app_inited ||
        frame_last.sd_state != sd_state ||
        frame_last.sys_state != sys_state)
    {
        uint8_t ind_state =
            sd_state == grbl_SDState_t::NotPresent ? IND_STATE_NONE :
            sd_state == grbl_SDState_t::Idle 	   ? IND_STATE_READY :
            sys_state == grbl_State_t::Hold ?
                IND_STATE_ENABLED : IND_STATE_ACTIVE;

        drawText(
            "S",
            win->just,
            win->font,
            win->x, win->y, win->w, win->h,
            indColor(ind_state),
            win->bg );
    }
}


void doWifiIndicator(const uiWin *win)
{
    uint8_t state = g_status.getWifiState();
    if (!app_inited || frame_last.wifi_state != state)
    {
        frame_last.wifi_state = state;
        drawText(
            "W",
            win->just,
            win->font,
            win->x, win->y, win->w, win->h,
            indColor(state),
            win->bg );
    }
}


void doMachinePosition(const uiWin *win)
{
    int idx = win->param;
    float *last_val = &frame_last.pos[idx];
    float val = g_status.m_machine_pos[idx];
    bool changed = !app_inited || *last_val != val;

    if (changed ||
        frame_last.prog_w != prog_w)
    {
        *last_val = val;
        char buf[12];
        sprintf(buf,"%3.1f",val);
        doTextProgress(win,buf,changed);
    }
}


void doWorkPosition(const uiWin *win)
{
    int idx = win->param;
    float *last_val = &frame_last.pos[UI_NUM_AXES + idx];
    float val = g_status.m_work_pos[idx];
    bool changed = !app_inited || *last_val != val;

    if (changed ||
        frame_last.prog_w != prog_w)
    {
        *last_val = val;
        char buf[12];
        sprintf(buf,"%3.1f",val);
        doTextProgress(win,buf,changed);
    }
}


void doSysState(const uiWin *win)
{
    bool changed = !app_inited || (frame_last.sys_state != sys_state);
    if (changed ||
        frame_last.prog_w != prog_w)
    {
        doTextProgress(win,sysStateName(sys_state),changed);
    }
}


void doMemAvail(const uiWin *win)
{
    uint16_t id = win->id;
    uint32_t avail = id == ID_MEMAVAIL ?
        xPortGetFreeHeapSize() :
        xPortGetMinimumEverFreeHeapSize();
    avail /= 1024;
    uint32_t *prev = id == ID_MEMAVAIL ?
        &frame_last.mem_avail :
        &frame_last.min_avail;
    bool changed = !app_inited || *prev != avail;
    if (changed ||
        frame_last.prog_w != prog_w)
    {
        *prev = avail;
        char buf[12];
        sprintf(buf,"%dK",avail);
        doTextProgress(win,buf,changed);
    }
}


void doJobProgress(const uiWin *win)
{
    float pct = g_status.filePct();

    if (!app_inited)        // implies pct=0, prog_x=0, prog_w=0, and frame_last.prog_w=0
    {
        tft.fillRect(
            win->x, win->y, win->w, win->h,
            win->bg);
    }

    // otherwise we fill the bar if the color has changed
    //

    else if (frame_last.prog_color != prog_color ||
             (sd_state == grbl_SDState_t::Busy &&
             frame_last.pct != pct))
    {
        prog_x = win->x;     // 0
        prog_w = ((pct * win->w)/100.0);
        if (frame_last.prog_w != prog_w)
        {
            tft.fillRect(
                prog_x + frame_last.prog_w, win->y, prog_w - frame_last.prog_w, win->h,
                prog_color);
        }
    }
}

//----------------------------------------------------------------------
// FRAME DEFINITION
//----------------------------------------------------------------------

const uiWin app_frame[] = {

    { ID_APP,            ID_NONE,        0,   0, 320, 240, doWin,             0,          COLOR_BLACK,    COLOR_WHITE, },
    { ID_TITLEBAR,       ID_APP,         0,   0, 320,  35, doWin,             0,          COLOR_DARKBLUE, COLOR_WHITE, },
    { ID_APP_BUTTON,     ID_TITLEBAR,    0,   0,  80,  35, doAppButton,       0,          COLOR_BLUE,     COLOR_WHITE, FONT_BIG,    JUST_CENTER, },
    { ID_APP_TITLE,      ID_TITLEBAR,   84,   0, 180,  35, doAppTitle,        0,          COLOR_DARKBLUE, COLOR_WHITE, FONT_NORMAL, JUST_CENTER, },
    { ID_SD_INDICATOR,   ID_TITLEBAR,  264,   0,  28,  35, doSDIndicator,     0,          COLOR_DARKBLUE, COLOR_WHITE, FONT_NORMAL, JUST_CENTER, },
    { ID_WIFI_INDICATOR, ID_TITLEBAR,  292,   0,  28,  35, doWifiIndicator,   0,          COLOR_DARKBLUE, COLOR_WHITE, FONT_NORMAL, JUST_CENTER, },
    { ID_STATUSBAR,      ID_APP,         0, 205, 320,  35, doJobProgress,     0,          COLOR_DARKBLUE, COLOR_WHITE, },
    { ID_MACHINE_X,      ID_STATUSBAR,  10, 205,  55,  16, doMachinePosition, UI_AXIS_X,  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_MACHINE_Y,      ID_STATUSBAR,  75, 205,  55,  16, doMachinePosition, UI_AXIS_Y,  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_MACHINE_Z,      ID_STATUSBAR, 140, 205,  50,  16, doMachinePosition, UI_AXIS_Z,  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_WORK_X,         ID_STATUSBAR,  10, 222,  55,  16, doWorkPosition,    UI_AXIS_X,  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_WORK_Y,         ID_STATUSBAR,  75, 222,  55,  16, doWorkPosition,    UI_AXIS_Y,  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_WORK_Z,         ID_STATUSBAR, 140, 222,  50,  16, doWorkPosition,    UI_AXIS_Z,  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_GRBL_STATE,     ID_STATUSBAR, 195, 205,  78,  35, doSysState,        0,          COLOR_DARKBLUE, COLOR_WHITE, FONT_NORMAL, JUST_CENTER, },
    { ID_MEMAVAIL,       ID_STATUSBAR, 277, 205,  42,  15, doMemAvail,        0,          COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_MEMAVAIL_MIN,   ID_STATUSBAR, 277, 222,  42,  15, doMemAvail,        0,          COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
};


#define NUM_FRAME_WINS   (sizeof(app_frame)/sizeof(uiWin))


//----------------------------------
// appUpdate()
//----------------------------------

void appUpdate()
{
    if (!app_inited)
        g_status.initWifiEventHandler();

    g_status.updateStatus();

    sys_state = g_status.getSysState();
    sd_state = g_status.getSDState();

    for (int i=0; i<NUM_FRAME_WINS; i++)
    {
        const uiWin *win = &app_frame[i];
        win->fn_cb(win);
    }

    app_inited = true;

    // update muliply used state change variables
    // if we went from busy to not busy, redraw the whole screen

    if (frame_last.sd_state != sd_state &&
        frame_last.sd_state == grbl_SDState_t::Busy)
    {
        app_inited = false;
        prog_x = 0;
        prog_w = 0;
        frame_last.prog_w = 0;
        prog_color = COLOR_BROWN;
    }

    // frame_last.job_state = job_state;
    frame_last.sd_state = sd_state;
    frame_last.sys_state = sys_state;
    frame_last.pct = g_status.filePct();
    frame_last.prog_w = prog_w;
    frame_last.prog_color = prog_color;
}
