//--------------------------------------
// A generic global static buffer
//--------------------------------------
// for use by the various windows who cache
// the version number to check if it's still
// up-to-date()

#pragma once

#include <cstdint>

#define MAX_G_BUFFER    2048

extern char         gs_buffer[MAX_G_BUFFER+1];
extern int          gs_buf_ptr;
extern uint32_t     gs_buffer_version;




