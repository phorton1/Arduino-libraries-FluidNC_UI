//--------------------------------------
// The Grbl_MinUI application class
//--------------------------------------

#pragma once

#include "gDefs.h"

#ifdef WITH_APPLICATION

    #define MAX_MACRO_PATH     32

    #define PREF_JOG_FEED_RATE  0
    #define PREF_MACRO1_CHAR    1
    #define PREF_MACRO2_CHAR    2
    #define PREF_MACRO3_CHAR    3
    #define PREF_MACRO4_CHAR    4
    #define PREF_MACRO1_PATH    5
    #define PREF_MACRO2_PATH    6
    #define PREF_MACRO3_PATH    7
    #define PREF_MACRO4_PATH    8

    void readPrefs();

    extern int  getIntPref(int id);
    extern const char *getStrPref(int id);

    extern void setIntPref(int id, int value);
    extern void setStrPref(int id, const char *value);

#endif  // WITH_APPLICATION
