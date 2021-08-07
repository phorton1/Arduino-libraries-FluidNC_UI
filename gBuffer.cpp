//--------------------------------------
// A generic global static buffer
//--------------------------------------
// for use by the currently active window only
// which re-initializes it on begin()

#include "gBuffer.h"

char gs_buffer[MAX_G_BUFFER+1] = {0};
uint32_t gs_buffer_version = 1;
int gs_buf_ptr = 0;