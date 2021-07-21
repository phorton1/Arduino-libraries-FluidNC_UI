//--------------------------------------
// GRBL application specific classes
//--------------------------------------

#include "gApp.h"
#include "Grbl_MinUI.h"     // for debug_serial()
#include "winIdle.h"
#include "winBusy.h"
#include "winAlarm.h"
#include "dlgConfirm.h"


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


gApplication    the_app;
winIdle         idle_win;
winBusy         busy_win;
winAlarm        alarm_win;
dlgConfirm      confirm_dlg;


char app_button_buf[UI_MAX_BUTTON + 1] = "MAIN";
char app_title_buf[UI_MAX_TITLE + 1] = "title";


uiMutable app_button = {
    app_button_buf,
    COLOR_BLUE,
    COLOR_WHITE,
    FONT_BIG
};



uiMutable app_title = {
    app_title_buf,
    COLOR_DARKBLUE,
    COLOR_WHITE,
    FONT_BIG
};


//----------------------------------------------------------------------
// FRAME DEFINITION
//----------------------------------------------------------------------

const uiElement app_elements[] =
{
    { ID_APP_BUTTON,         0,   0,  80,  35,   V(&app_button) },
    { ID_APP_TITLE,         84,   0, 180,  35,   V(&app_title) },
    { ID_SD_INDICATOR,     264,   0,  28,  35,   0,             COLOR_DARKBLUE, COLOR_WHITE, FONT_NORMAL, JUST_CENTER, },
    { ID_WIFI_INDICATOR,   292,   0,  28,  35,   0,             COLOR_DARKBLUE, COLOR_WHITE, FONT_NORMAL, JUST_CENTER, },
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



gApplication::gApplication() :
    uiWindow(app_elements,sizeof(app_elements)/sizeof(uiElement))
{
}


void gApplication::begin()
{
    draw_needed = true;
    g_status.initWifiEventHandler();
}


void gApplication::setTitle(const char *text)
{
    int len = strlen(text);
    if (len > UI_MAX_TITLE) len = UI_MAX_TITLE;
    memcpy(app_title_buf, text,len);
    app_title_buf[len] = 0;
}


void gApplication::setCurWindow(uiWindow *win)
{
    tft.fillRect(0,UI_TOP_MARGIN,UI_SCREEN_WIDTH,UI_SCREEN_HEIGHT-UI_TOP_MARGIN-UI_BOTTOM_MARGIN,COLOR_BLACK);
    cur_window = win;
    cur_window->begin();
}



void gApplication::confirmCommand(uint16_t command)
{
        confirm_dlg.setMessage(
            command == CONFIRM_COMMAND_RESET ? "reset the GRBL Machine?" :
            command == CONFIRM_COMMAND_REBOOT ? "reboot the Controller?" :
            "UNKNOWN CONFIRM COMMAND");

        pending_command = command;
        prev_window = cur_window;
        setCurWindow(&confirm_dlg);
}

void gApplication::endConfirm(uint16_t rslt)
{
    if (rslt)
    {
        if (pending_command == CONFIRM_COMMAND_RESET)
        {
            #if !TEST_STANDALONE_UI
                execute_realtime_command(Cmd::Reset,CLIENT_ALL);
                    // CLIENT_TOUCH_UI);
            #endif
        }
        else if (pending_command == CONFIRM_COMMAND_REBOOT)
        {
            debug_serial("gApplication estarting the ESP32!!");
            delay(500);
            ESP.restart();
            while (1) {}
        }
    }
    setCurWindow(prev_window);
}


//------------------------
// utilities
//------------------------

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


//--------------------------------
// frame callbacks
//--------------------------------


void gApplication::doAppButton(const uiElement *ele)
{
    float pct = g_status.filePct();
    grbl_State_t sys_state = g_status.getSysState();
    grbl_SDState_t sd_state = g_status.getSDState();

    if (last.sd_state != sd_state ||
         last.sys_state != sys_state ||
         last.pct != pct)
    {
        if (sd_state == grbl_SDState_t::Busy)
        {
            sprintf(app_button_buf,"%2.1f%%",pct);
            app_button.fg = sys_state == grbl_State_t::Hold ?
                COLOR_CYAN : COLOR_YELLOW;
        }
        else
        {
            app_button.fg = COLOR_WHITE;
            strcpy(app_button_buf,"MAIN");
        }

        drawTypedElement(ele,false);
    }
}


void gApplication::doAppTitle(const uiElement *ele)
{
    grbl_State_t sys_state = g_status.getSysState();
    grbl_SDState_t sd_state = g_status.getSDState();

    if (last.sd_state != sd_state ||
        last.sys_state != sys_state ||
        strcmp(app_title_buf,last.app_title))
    {
        // PRELIMINARY WINDOW MANAGMENT - WIP
        // this is where we start a "Job"

        // debug_serial("doAppTitle sys(%d) sd(%d)",sd_state,sys_state);

        if (last.sd_state != sd_state &&
            sd_state == grbl_SDState_t::Busy)
        {
            const char *filename = g_status.getActiveFilename();
            const char *p = filename;
            while (*p)
            {
                if (*p++ == '/')
                filename = p;
            }
            setTitle(filename);
            setCurWindow(&busy_win);
        }

        // PRELIMINARY WINDOW MANAGMENT - WIP
        // and this is where we  potentially change from "Busy" to "Hold"

        if (sd_state == grbl_SDState_t::Busy)
        {
            app_title.fg = sys_state == grbl_State_t::Hold ?
                COLOR_CYAN : COLOR_YELLOW;
            prog_color = sys_state == grbl_State_t::Hold ?
                COLOR_DARKCYAN : COLOR_BROWN;

            // invalidate the progress bar upon a color change

            if (last.prog_color != prog_color)
            {
                last.prog_w = 0;
            }
        }
        else
        {
            app_title.fg = COLOR_WHITE;
            strcpy(app_title_buf,"NO JOB");
        }

        drawTypedElement(ele,false);
    }
}


void gApplication::doSDIndicator(const uiElement *ele)
{
    grbl_State_t sys_state = g_status.getSysState();
    grbl_SDState_t sd_state = g_status.getSDState();

    if (draw_needed ||
        last.sd_state != sd_state ||
        last.sys_state != sys_state)
    {
        uint8_t ind_state =
            sd_state == grbl_SDState_t::NotPresent ? IND_STATE_NONE :
            sd_state == grbl_SDState_t::Idle 	   ? IND_STATE_READY :
            sys_state == grbl_State_t::Hold ?
                IND_STATE_ENABLED : IND_STATE_ACTIVE;

        drawText(
            "S",
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
            "W",
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
    grbl_State_t sys_state = g_status.getSysState();
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


void gApplication::doJobProgress(const uiElement *ele)
{
    float pct = g_status.filePct();
    grbl_SDState_t sd_state = g_status.getSDState();

    if (draw_needed)  // implies pct=0, prog_x=0, prog_w=0, and last.prog_w=0
    {
        tft.fillRect(
            ele->x, ele->y, ele->w, ele->h,
            ele->bg);
    }

    // otherwise we fill the bar if the color has changed

    else if (last.prog_color != prog_color ||
             (sd_state == grbl_SDState_t::Busy &&
             last.pct != pct))
    {
        prog_x = ele->x;     // 0
        prog_w = ((pct * ele->w)/100.0);
        if (last.prog_w != prog_w)
        {
            tft.fillRect(
                prog_x + last.prog_w, ele->y, prog_w - last.prog_w, ele->h,
                prog_color);
        }
    }
}



//----------------------------------
// appUpdate()
//----------------------------------

void gApplication::update()
{
    g_status.updateStatus();

    if (draw_needed)
    {
        tft.fillRect(0,0,UI_SCREEN_WIDTH,UI_SCREEN_HEIGHT,COLOR_BLACK);
        tft.fillRect(0,0,UI_SCREEN_WIDTH,UI_TOP_MARGIN,COLOR_DARKBLUE);
        drawTypedElements();
        setCurWindow(&idle_win);
    }

    uiWindow::updateTouch();
    hitTest();

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

    // dispatch to child window if any

    if (cur_window)
    {
        cur_window->hitTest();
        cur_window->update();
    }

    // PRELIMINARY WINDOW MANAGMENT - WIP
    // we have drawn everything that needs to be drawn
    // except for below we notice going "out" of "busy
    // mode and will switch to the idle window and redraw

    draw_needed = false;

    // update muliply used state change variables
    // if we went from busy to not busy, redraw the whole screen

    grbl_State_t sys_state = g_status.getSysState();
    grbl_SDState_t sd_state = g_status.getSDState();

    if (last.sd_state != sd_state &&
        last.sd_state == grbl_SDState_t::Busy)
    {
        prog_x = 0;
        prog_w = 0;
        last.prog_w = 0;
        prog_color = COLOR_BROWN;

        // re-draw the whole window
        // and return to the idle window

        draw_needed = true;
    }

    last.sd_state = sd_state;
    last.sys_state = sys_state;
    last.pct = g_status.filePct();
    last.prog_w = prog_w;
    last.prog_color = prog_color;
    strcpy(last.app_title,app_title_buf);
}
