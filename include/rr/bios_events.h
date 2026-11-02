#ifndef RR_BIOS_EVENTS_H
#define RR_BIOS_EVENTS_H
#include "types.h"
/* B0 event/file services8,0C and32, respectively. Event handles are machine
 * words, not memory pointers. Callback pointers retain their native width. */
typedef u32 RrBiosEventHandle;
typedef s32 (*RrBiosEventCallback)(void);
RrBiosEventHandle func_80096E74(u32 eventClass,u32 specification,u32 mode,
    RrBiosEventCallback callback);
s32 func_80096EB4(RrBiosEventHandle handle);
s32 func_80096EA4(RrBiosEventHandle handle);
s32 func_80096F44(s32 descriptor,void *buffer,s32 count);
s32 func_80096F24(const char *name,s32 mode);
#endif
