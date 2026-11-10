#ifndef RR_SERIAL_HANDLES_H
#define RR_SERIAL_HANDLES_H
#include "rr/driver_control.h"

/* BIOS event IDs are opaque 32-bit handles; file descriptors remain signed. */
extern u32 ovl667_sio_read_event,ovl667_sio_error_event;
extern s32 LinkWriteHandle,LinkReadHandle;
s32 DisableEvent(u32 handle);
s32 CloseEvent(u32 handle);
s32 FileClose(s32 descriptor);
void ovl667_close_sio_session(void);
#endif
