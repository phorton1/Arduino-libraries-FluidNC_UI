#pragma once


#include "gDefs.h"


#ifdef WITH_GRBL
    #include <Report.h>
#else
    void debug_serial(const char *format, ...);
        // in _testGrblUI.ino
#endif


extern void Grbl_MinUI_init();
