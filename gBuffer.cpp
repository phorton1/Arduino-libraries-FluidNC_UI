//--------------------------------------
// A generic global static buffer
//--------------------------------------
// This general purpose large buffer can be used
// by any window.  It is re-initialized as neessary
// when changing windows from the window's begin()
// method. Clients should bump the version number
// so that other windows may know the contents have
// been changed.
//
// Currently only used by uiScrollable->winFiles.

#include "gBuffer.h"

char gs_buffer[MAX_G_BUFFER+1] = {0};
uint32_t gs_buffer_version = 1;
int gs_buf_ptr = 0;