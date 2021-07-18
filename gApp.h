//--------------------------------------
// GRBL application specific classes
//--------------------------------------
// gApplication, gTitleBar, gContentArea, and gStatusBar

#pragma once

#include "gStatus.h"

enum jobState_t
{
    JOB_STATE_NONE = 0,
    JOB_STATE_STARTED,
    JOB_STATE_PAUSED,
    JOB_STATE_COMPLETE,
    JOB_STATE_ABORTED,
    JOB_STATE_ERROR
};


extern const char *jobStateName(jobState_t state);

extern void appUpdate();
