#pragma once


#include "gDefs.h"


extern void g_debug(const char *format, ...);

#ifdef WITH_INIT_UI
    extern void Grbl_MinUI_init();
#endif
