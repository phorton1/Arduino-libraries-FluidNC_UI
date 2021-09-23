//-------------------------------------------------------
// An object that abstracts the state of FluidNC
//-------------------------------------------------------
// Hides the underlying FluidNC impelementation from
// the UI. Is called once per refresh cycle.  Has a scheme
// for primary UI objects to detect state changes.
// Currently implemented to maintain state of a 3 axis machine.
// The AXIS definitions must agree with those in grbl_esp32.

#pragma once

#include <Arduino.h>    // for String type
#include "gDefs.h"

// duplicated from FluidNC for isolation

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


// enum class grbl_ProgramFlow_t : uint8_t {
//     Running      = 0,   // (Default: Must be zero)
//     Paused       = 3,   // M0
//     OptionalStop = 1,   // M1 NOTE: Not supported, but valid and ignored.
//     CompletedM2  = 2,   // M2 (Do not alter value)
//     CompletedM30 = 30,  // M30 (Do not alter value)
// };


extern const char *sysStateName(grbl_State_t state);
extern const char *sdStateName(grbl_SDState_t state);




class gStatus
{
public:

    gStatus()   {}

    void initWifiEventHandler();
    void updateStatus();
    void gWifiEvent(uint16_t event);

    uint8_t getWifiState()          { return m_wifi_state; }
    grbl_State_t getSysState()      { return m_sys_state; }
    grbl_SDState_t getSDState(bool refresh=false);

    const char* getActiveFilename() { return m_active_filename; }
    float filePct()                 { return m_file_pct; }

    // public state variables

    int32_t m_sys_pos[UI_NUM_AXES];
    float m_machine_pos[UI_NUM_AXES];
    float m_work_pos[UI_NUM_AXES];

protected:

    // state variables

    uint8_t m_wifi_state;
    grbl_State_t m_sys_state = grbl_State_t::Sleep;
    grbl_SDState_t m_sdcard_state = grbl_SDState_t::NotPresent;

    const char *m_active_filename;
    float m_file_pct;

    // implementation variables

    bool m_started = false;
    // uint32_t m_sd_card_timer = 0;

};  // class gStatus


extern gStatus g_status;
