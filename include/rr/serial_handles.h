#ifndef RR_SERIAL_HANDLES_H
#define RR_SERIAL_HANDLES_H
#include "rr/driver_control.h"

/* BIOS event IDs are opaque 32-bit handles; file descriptors remain signed. */
extern u32 ovl667_D_80118ED8,ovl667_D_80118EDC;
extern s32 D_800F2AAC,D_800F2AA8;
s32 func_80096EC4(u32 handle);
s32 func_80096E84(u32 handle);
s32 func_80096F64(s32 descriptor);
void ovl667_func_80114A40(void);
#endif
