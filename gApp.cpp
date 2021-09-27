//--------------------------------------
// Application specific classes
//--------------------------------------

#include "gApp.h"
#include "gPrefs.h"
#include "winMain.h"
#include "dlgMsg.h"
#include "dlgMainMenu.h"

#ifdef UI_WITH_MESH
    #include <Mesh.h>          // FluidNC_extensions
#endif


#define DEBUG_APP   1


#define MAX_WINDOW_STACK  5
#define MAX_ACTIVE_FILENAME 80
// IDS OF ANY PARENTS MUST BE GLOBALLY UNIQUE !!

#define ID_APP_BUTTON           (0x0021 | ID_TYPE_TEXT | ID_TYPE_BUTTON | ID_TYPE_MUTABLE)
#define ID_APP_TITLE            (0x0022 | ID_TYPE_TEXT | ID_TYPE_MUTABLE)
#define ID_SD_INDICATOR         (0x0023)
#define ID_WIFI_INDICATOR       (0x0024)

#define ID_STATUSBAR            (0x0030)
#define ID_MACHINE_X            (0x0031)
#define ID_MACHINE_Y            (0x0032)
#define ID_MACHINE_Z            (0x0033)
#define ID_WORK_X               (0x0034)
#define ID_WORK_Y               (0x0035)
#define ID_WORK_Z               (0x0036)
#define ID_GRBL_STATE           (0x0037)
#define ID_MEMAVAIL             (0x0038)
#define ID_MEMAVAIL_MIN         (0x0039)


gApplication the_app;

static uint8_t win_stack_ptr = 0;
static uiWindow *win_stack[MAX_WINDOW_STACK] = {0};

static char app_button_buf[UI_MAX_BUTTON + 1] = "MAIN";
static char app_title_buf[UI_MAX_TITLE + 1] = "";
static char active_filename[MAX_ACTIVE_FILENAME + 1] = "";


//----------------------------------------------------------------------
// FRAME DEFINITION
//----------------------------------------------------------------------

static uiMutable app_button = {
    app_button_buf,
    COLOR_BLUE,
    COLOR_WHITE,
    FONT_BIG
};

static uiMutable app_title = {
    app_title_buf,
    COLOR_DARKBLUE,
    COLOR_WHITE,
    FONT_SMALL
};

#define CHAR_WIFI_SYMBOL  "\x20"
#define CHAR_SD_SYMBOL    "\x21"

static const uiElement app_elements[] =
{
    { ID_APP_BUTTON,         0,   0,  90,  35,   V(&app_button) },
    { ID_APP_TITLE,         94,   0, 180,  35,   V(&app_title)  },
    { ID_SD_INDICATOR,     274,   0,  23,  35,   0,             COLOR_DARKBLUE, COLOR_WHITE, FONT_SYMBOL, JUST_CENTER, },
    { ID_WIFI_INDICATOR,   297,   0,  23,  35,   0,             COLOR_DARKBLUE, COLOR_WHITE, FONT_SYMBOL, JUST_CENTER, },
    { ID_STATUSBAR,          0, 205, 320,  35,   0,             COLOR_DARKBLUE, COLOR_WHITE, },
    { ID_MACHINE_X,         10, 205,  55,  16,   V(UI_AXIS_X),  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_MACHINE_Y,         75, 205,  55,  16,   V(UI_AXIS_Y),  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_MACHINE_Z,        140, 205,  50,  16,   V(UI_AXIS_Z),  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_WORK_X,            10, 222,  55,  16,   V(UI_AXIS_X),  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_WORK_Y,            75, 222,  55,  16,   V(UI_AXIS_Y),  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_WORK_Z,           140, 222,  50,  16,   V(UI_AXIS_Z),  COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_GRBL_STATE,       195, 205,  78,  35,   0,             COLOR_DARKBLUE, COLOR_WHITE, FONT_NORMAL, JUST_CENTER, },
    { ID_MEMAVAIL,         277, 205,  42,  15,   0,             COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
    { ID_MEMAVAIL_MIN,     277, 222,  42,  15,   0,             COLOR_DARKBLUE, COLOR_WHITE, FONT_MONO,   JUST_RIGHT,  },
};


//----------------------------
// implementation
//----------------------------


gApplication::gApplication() :
    uiWindow(app_elements,sizeof(app_elements)/sizeof(uiElement))
{
}


void gApplication::begin()
{
    #if DEBUG_APP
        delay(400);  // more attempt to stop garbling output
        g_debug("gApplication::begin() started ...");
    #endif

    readPrefs();
    draw_needed = true;
    suppress_status = false;
    g_status.initWifiEventHandler();
    setDefaultWindow((uiWindow *)&main_win);

    #if DEBUG_APP
        g_debug("gApplication::begin() finished.");
    #endif
}



void gApplication::openWindow(uiWindow *win)
{
    if (win->isModal())
    {
        win_stack_ptr++;
        g_debug("openWindow(modal) stack=%d",win_stack_ptr);
        if (win_stack_ptr>1)
        {
            strcpy(app_button_buf,"back");
            drawTypedElement(&app_elements[0]);
        }
    }
    win_stack[win_stack_ptr] = win;
    win->begin();
}


void gApplication::endModal()
{
    if (win_stack[win_stack_ptr]->isModal())
    {
        g_debug("endModal stack=%d",win_stack_ptr);
        setTitle("");

        draw_needed = true;
        win_stack_ptr--;
        suppress_status = false;
        win_stack[win_stack_ptr]->begin();
    }

}

void gApplication::setBaseWindow(uiWindow *win)
{
    win_stack_ptr = 0;
    setAppButtonText(win->getMenuLabel());
    openWindow(win);
}



void gApplication::onButton(const uiElement *ele, bool pressed)
{
    if (!pressed)
    {
        if (win_stack[win_stack_ptr]->isModal())
            endModal();
        else
            openWindow(&main_menu);
    }
}


//------------------------
// utilities
//------------------------

const char *gApplication::getAppButtonText()
{
    return app_button_buf;
}


void gApplication::setAppButtonText(const char *text)
{
    int len = strlen(text);
    if (len > UI_MAX_BUTTON) len = UI_MAX_BUTTON;
    memcpy(app_button_buf,text,len);
    app_button_buf[len] = 0;
}


void gApplication::setTitle(const char *text)
{
    int len = strlen(text);
    if (len > UI_MAX_TITLE) len = UI_MAX_TITLE;
    memcpy(app_title_buf, text,len);
    app_title_buf[len] = 0;
}


uint16_t gApplication::indColor(uint8_t ind_state)
{
    return
        ind_state == IND_STATE_ENABLED ? COLOR_CYAN :
        ind_state == IND_STATE_READY   ? COLOR_GREEN :
        ind_state == IND_STATE_ACTIVE  ? COLOR_YELLOW :
        ind_state == IND_STATE_ERROR   ? COLOR_RED :
        COLOR_LIGHTGREY;
}


void gApplication::doText(const uiElement *ele, const char *text)
{
    drawText(
        text,
        ele->just,
        ele->font,
        ele->x, ele->y, ele->w, ele->h,
        ele->fg,
        ele->bg );
}


//--------------------------------
// frame callbacks
//--------------------------------

void gApplication::doAppButton(const uiElement *ele)
{
    uiWindow *win = win_stack[0];
    bool is_main_win = win == ((uiWindow *)&main_win);

    float pct = g_status.filePct();
    bool busy_or_hold =
        job_state == JOB_BUSY ||
        job_state == JOB_HOLD;
    bool pct_changed =
        is_main_win &&
        busy_or_hold &&
        last.pct != pct;

    if (draw_needed ||
        pct_changed ||
        win != last.window ||
        job_state != last.job_state)
    {
        if (is_main_win && busy_or_hold)
            sprintf(app_button_buf,"%2.1f%%",pct);
        else // if (!busy_or_hold)
            strcpy(app_button_buf,win->getMenuLabel());

        app_button.fg =
            job_state == JOB_HOLD ? COLOR_CYAN :
            job_state != JOB_IDLE ? COLOR_YELLOW :
            COLOR_WHITE;

        drawTypedElement(ele,false);
    }
}


void gApplication::doAppTitle(const uiElement *ele)
{
    if (draw_needed ||
        job_state != last.job_state ||
        strcmp(app_title_buf,last.app_title))
    {
        // the title, whatever it happens to be
        // stays red when alarm, yellow when busy,
        // cyan when hold, or white when idle

        app_title.fg =
            job_state == JOB_HOMING || job_state == JOB_ALARM ? COLOR_YELLOW :  // COLOR_RED :
            job_state == JOB_HOLD ? COLOR_CYAN :
            job_state == JOB_BUSY ? COLOR_YELLOW :
            COLOR_WHITE;


        drawTypedElement(ele,false);
    }
}


void gApplication::doSDIndicator(const uiElement *ele)
{
    if (draw_needed || last.sd_state != sd_state)
    {
        uint8_t ind_state =
            sd_state == SDState::NotPresent ? IND_STATE_NONE :
            sd_state == SDState::Idle   	? IND_STATE_READY :
                IND_STATE_ACTIVE;
        drawText(
            CHAR_SD_SYMBOL,
            ele->just,
            ele->font,
            ele->x, ele->y, ele->w, ele->h,
            indColor(ind_state),
            ele->bg );
    }
}


void gApplication::doWifiIndicator(const uiElement *ele)
{
    uint8_t state = g_status.getWifiState();
    if (draw_needed || last.wifi_state != state)
    {
        last.wifi_state = state;
        drawText(
            CHAR_WIFI_SYMBOL,
            ele->just,
            ele->font,
            ele->x, ele->y, ele->w, ele->h,
            indColor(state),
            ele->bg );
    }
}


void gApplication::doMachinePosition(const uiElement *ele)
{
    int idx = (int) ele->param;
    float *last_val = &last.pos[idx];
    float val = g_status.m_machine_pos[idx];
    bool changed = draw_needed || *last_val != val;

    if (changed ||
        last.prog_w != prog_w)
    {
        *last_val = val;
        char buf[12];
        sprintf(buf,"%3.1f",val);
        doTextProgress(ele,buf,changed);
    }
}


void gApplication::doWorkPosition(const uiElement *ele)
{
    int idx = (int) ele->param;
    float *last_val = &last.pos[UI_NUM_AXES + idx];
    float val = g_status.m_work_pos[idx];
    bool changed = draw_needed || *last_val != val;

    if (changed ||
        last.prog_w != prog_w)
    {
        *last_val = val;
        char buf[12];
        sprintf(buf,"%3.1f",val);
        doTextProgress(ele,buf,changed);
    }
}


void gApplication::doSysState(const uiElement *ele)
{
    bool changed = draw_needed || (last.sys_state != sys_state);
    if (changed ||
        last.prog_w != prog_w)
    {
        doTextProgress(ele,sysStateName(sys_state),changed);
    }
}


void gApplication::doMemAvail(const uiElement *ele)
{
    uint16_t id_type = ele->id_type;
    uint32_t avail = id_type == ID_MEMAVAIL ?
        xPortGetFreeHeapSize() :
        xPortGetMinimumEverFreeHeapSize();
    avail /= 1024;
    uint32_t *prev = id_type == ID_MEMAVAIL ?
        &last.mem_avail :
        &last.min_avail;
    bool changed = draw_needed || *prev != avail;
    if (changed ||
        last.prog_w != prog_w)
    {
        *prev = avail;
        char buf[12];
        sprintf(buf,"%dK",avail);
        doTextProgress(ele,buf,changed);
    }
}



//-------------------------
// progress bar
//-------------------------

void gApplication::initProgress()
{
    prog_x = 0;
    prog_w = 0;
    last.prog_w = 0;
    #if DEBUG_APP
        g_debug("initProgress()");
    #endif
}


void gApplication::doTextProgress(const uiElement *ele, const char *text, bool changed)
    // Called for fields that lie on the progress bar to draw the text.
    // Called if the field has changed or last.prog_w != prog_w,
    // which implies that a new bar portion has been drawn.
{
    // develop s,e notation

    int16_t win_s = ele->x;
    int16_t win_e = ele->x + ele->w - 1;
    int16_t prog_s = prog_x + last.prog_w;
    int16_t prog_e = prog_x + prog_w - 1;

    // win_s > prog_e implies that there is no progress bar or
    // the field lays completely to the right of the progress bar
    // so we only write the value if it changed using default background

    if (win_s > prog_e)
    {
        if (changed)
            doText(ele,text);
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
        tft.fillRect(prog_s,ele->y,bar_w,ele->h,prog_color);
    if (normal_w > 0)
        tft.fillRect(normal_s,ele->y,normal_w,ele->h,ele->bg);

    // Finally ... draw the text with no background color

    drawText(
        text,
        ele->just,
        ele->font,
        ele->x, ele->y, ele->w, ele->h,
        ele->fg,
        ele->fg );      // bg <= fg implies no background
}


void gApplication::doJobProgress(const uiElement *ele)
{
    if (draw_needed)
    {
        tft.fillRect(
            ele->x, ele->y, ele->w, ele->h,
            ele->bg);
    }

    // we fill the bar if the color has changed
    // or the percent has changed

    float pct = job_state == JOB_HOLD || job_state == JOB_BUSY ? g_status.filePct() : 0;
    prog_color = job_state == JOB_HOLD ? COLOR_DARKCYAN : COLOR_DARKGREEN;

    if (last.pct != pct ||
        last.prog_color != prog_color)
    {
        // invalidate the progress bar upon a color change

        if (last.prog_color != prog_color)
            last.prog_w = 0;

        prog_x = ele->x;     // 0
        prog_w = ((pct * ele->w)/100.0);

        // draw from last.prog_w to prog_w

        if (last.prog_w != prog_w)
        {
            tft.fillRect(
                prog_x + last.prog_w, ele->y, prog_w - last.prog_w, ele->h,
                prog_color);
        }
    }
}



//----------------------------------
// update()
//----------------------------------

void gApplication::setDefaultWindow(uiWindow *win)
    // set window without doing openWindow
    // which is deferred
{
    win_stack_ptr = 0;
    win_stack[0] = win;
    setAppButtonText(win->getMenuLabel());
}


void gApplication::update()
{
    bool job_finished = false;

    // update the gStatus object

    g_status.updateStatus(
        #ifdef UI_WITH_MESH
            the_mesh.inLeveling()
        #endif
    );

    // copy the state into gApplication variables that are
    // valid for one time through update()

    sys_state = g_status.getSysState();
    sd_state = g_status.getSDState();
    job_state = g_status.getJobState();


    if (job_state != last.job_state)
    {
        #if DEBUG_APP
            g_debug("UI JOB_STATE changing from %d to %d",last.job_state, job_state);
        #endif

        // determine if we need to initProgress() and begin() the window again

        bool new_win = false;
        if (job_state == JOB_ALARM)
            new_win = true;
        else if (job_state == JOB_BUSY && last.job_state != JOB_HOLD)
            new_win = true;
        else if (job_state == JOB_IDLE && (
            last.job_state == JOB_ALARM ||
            last.job_state == JOB_BUSY ||
            last.job_state == JOB_HOLD ))
            new_win = true;

        // the above *almost* connotes a job well done
        // we presume going from BUSY directly to IDLE
        // means just that.

        if (new_win)
        {
            draw_needed = true;
            initProgress();
            setDefaultWindow((uiWindow *)&main_win);

            if (job_state == JOB_IDLE)
            {
                if (last.job_state == JOB_BUSY)
                    job_finished = true;
            }
        }

        if (job_state == JOB_BUSY ||
            job_state == JOB_HOLD)
        {
            const char *filename = g_status.getActiveFilename();
            const char *p = filename;
            while (*p)
            {
                if (*p++ == '/')
                filename = p;
            }

            int len = strlen(filename);
            if (len > MAX_ACTIVE_FILENAME) len = MAX_ACTIVE_FILENAME;
            memcpy(active_filename,filename,len);
            active_filename[len] = 0;
            setTitle(active_filename);
        }
    }

    // hmmm ... do hit testing before checking draw_needed
    // as tft_calibration may set it ..

    uiWindow::updateTouch();
    hitTest();

    // redraw the whole window if necessary

    if (draw_needed)
    {
        tft.fillRect(0,0,UI_SCREEN_WIDTH,UI_SCREEN_HEIGHT,COLOR_BLACK);
        tft.fillRect(0,0,UI_SCREEN_WIDTH,UI_TOP_MARGIN,COLOR_DARKBLUE);
        drawTypedElements();
        // openWindow();
        win_stack[win_stack_ptr]->begin();
    }

    // but do the hitTests after the window is drawn?


    // dispatch to child window if any

    if (win_stack[win_stack_ptr])
    {
        win_stack[win_stack_ptr]->hitTest();

        // press outside of main menu closes it

        if (g_pressed &&
            !g_win_pressed &&
            win_stack[win_stack_ptr] == &main_menu)
        {
            endModal();
        }
        else
        {
            win_stack[win_stack_ptr]->update();
        }
    }


    // dispatch to frame elements

    for (int i=0; i<m_num_elements; i++)
    {
        const uiElement *ele = &m_elements[i];
        switch (ele->id_type)
        {
            case ID_APP_BUTTON:         doAppButton(ele);       break;
            case ID_APP_TITLE:          doAppTitle(ele);        break;
            case ID_SD_INDICATOR:       doSDIndicator(ele);     break;
            case ID_WIFI_INDICATOR:     doWifiIndicator(ele);   break;
        }

        if (!suppress_status)
        {
            switch (ele->id_type)
            {
                case ID_STATUSBAR:          doJobProgress(ele);     break;
                case ID_MACHINE_X:
                case ID_MACHINE_Y:
                case ID_MACHINE_Z:          doMachinePosition(ele); break;
                case ID_WORK_X:
                case ID_WORK_Y:
                case ID_WORK_Z:             doWorkPosition(ele);    break;
                case ID_GRBL_STATE:         doSysState(ele);        break;
                case ID_MEMAVAIL:
                case ID_MEMAVAIL_MIN:       doMemAvail(ele);        break;
            }
        }
    }

    // save state for next time through update()

    draw_needed = false;
    last.sys_state = sys_state;
    last.sd_state = sd_state;
    last.job_state = job_state;
    last.pct = job_state == JOB_HOLD || job_state == JOB_BUSY ? g_status.filePct() : 0;
    last.prog_w = prog_w;
    last.prog_color = prog_color;
    strcpy(last.app_title,app_title_buf);
    last.window = win_stack[0];

    if (job_finished)
    {
        doToast(COLOR_GREEN,"finished printing",active_filename);
        setTitle("");
    }
}


const char *gApplication::getActiveFilename()
{
    return active_filename;
}
