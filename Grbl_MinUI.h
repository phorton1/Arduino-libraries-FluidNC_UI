#pragma once

// This define allows you to run the UI without linking in grbl_Esp32.
// Your program must provide the implementation of debug_serial()

#define TEST_STANDALONE_UI   1

#if TEST_STANDALONE_UI
    void debug_serial(const char *format, ...);
        // in _testGrblUI.ino
#else
    #include <Report.h>
#endif


extern void Grbl_MinUI_init();
