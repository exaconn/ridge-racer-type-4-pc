#ifndef RR_BIOS_EVENTS_H
#define RR_BIOS_EVENTS_H
#include "types.h"
/* B0 event/file services8,0C and32, respectively. Event handles are machine
 * words, not memory pointers. Callback pointers retain their native width. */
typedef u32 BiosEventHandle;
typedef s32 (*BiosEventCallback)(void);
BiosEventHandle OpenEvent(u32 eventClass,u32 specification,u32 mode,
    BiosEventCallback callback);
s32 EnableEvent(BiosEventHandle handle);
s32 TestEvent(BiosEventHandle handle);
s32 FileRead(s32 descriptor,void *buffer,s32 count);
s32 FileOpen(const char *name,s32 mode);
#endif
