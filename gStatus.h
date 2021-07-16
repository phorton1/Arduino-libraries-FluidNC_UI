//-------------------------------------------------------
// An object that abstracts the state of Grbl_Esp32
//-------------------------------------------------------
// Hides the underlying Grbl_Esp32 impelementation from
// the UI. Is called once per refresh cycle.  Has a scheme
// for primary UI objects to detect state changes.
// Currently implemented to maintain state of a 3 axis machine.
// The AXIS definitions must agree with those in grbl_esp32.

#pragma once

#include <Arduino.h>    // for String type
#include "gDefs.h"


// the primary modes of the "main" window

enum ui_main_mode_t
{
    MAIN_MODE_NONE = 0,
    MAIN_MODE_IDLE,
    MAIN_MODE_BUSY,
    MAIN_MODE_ALARM
};

// once a job has started it must go to
// completion, abort, or error.

enum ui_job_state_t
{
    JOB_STATE_NONE = 0,
    JOB_STATE_STARTED,
    JOB_STATE_PAUSED,
    JOB_STATE_COMPLETE,
    JOB_STATE_ABORTED,
    JOB_STATE_ERROR
};

// duplicated from GRBL for isolation

enum class grbl_State_t : uint8_t {
    Idle = 0,    // Must be zero.
    Alarm,       // In alarm state. Locks out all g-code processes. Allows settings access.
    CheckMode,   // G-code check mode. Locks out planner and motion only.
    Homing,      // Performing homing cycle
    Cycle,       // Cycle is running or motions are being executed.
    Hold,        // Active feed hold
    Jog,         // Jogging mode.
    SafetyDoor,  // Safety door is ajar. Feed holds and de-energizes system.
    Sleep,       // Sleep state.
};

enum class grbl_SDState_t : uint8_t {
    Idle          = 0,
    NotPresent    = 1,
    Busy          = 2,
    BusyUploading = 3,
    BusyParsing   = 4,
};


extern const char *uiMainModeName(ui_main_mode_t mode);
extern const char *uiJobStateName(ui_job_state_t);
extern const char *uiSystemStateName(grbl_State_t state);
extern const char *uiSDCardStateName(grbl_SDState_t state);


class gStatus
{
public:

    gStatus()   {}

    void initWifiEventHandler();
        // called from gApplication::ctor()
    void updateStatus();
        // called from gApplication::update()
    void gWifiEvent(uint16_t event);
        // called from static onWifiEvent() method

    ui_main_mode_t getMainMode()        { return m_main_mode; }

    grbl_State_t getSystemState()       { return m_sys_state; }

    grbl_SDState_t getSDCardState()     { return m_sdcard_state; }

    ui_job_state_t getJobState()        { return m_job_state; }
    const char* getActiveFilename()     { return m_active_filename.c_str(); }
    float filePct()                     { return m_file_pct; }

    uint16_t getWifiState()             { return m_wifi_state; }

    // public state variables

    int32_t m_sys_pos[UI_NUM_AXES];
    float m_machine_pos[UI_NUM_AXES];
    float m_work_pos[UI_NUM_AXES];


protected:

    // state variables

    ui_main_mode_t m_main_mode = MAIN_MODE_NONE;

    grbl_State_t m_sys_state = grbl_State_t::Sleep;

    grbl_SDState_t m_sdcard_state = grbl_SDState_t::NotPresent;

    ui_job_state_t m_job_state = JOB_STATE_NONE;
    String m_active_filename;
    float m_file_pct = 0;

    uint16_t m_wifi_state;

    // implementation variables

    bool m_started = false;
    uint32_t m_sd_card_timer = 0;



};  // class gStatus


extern gStatus g_status;
