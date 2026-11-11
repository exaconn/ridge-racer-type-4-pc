#ifndef RR_BIOS_EVENTS_H
#define RR_BIOS_EVENTS_H
#include "types.h"
/* B0 event/file services8,0C and32, respectively. Event handles are machine
 * words, not memory pointers. Callback pointers retain their native width. */
typedef u32 BiosEventHandle;
typedef s32 (*BiosEventCallback)(void);
/* PsyZ kernel: long OpenEvent(unsigned long desc, long spec, long mode,
 * long (*func)()), long EnableEvent(long), long TestEvent(long). Handles
 * (DescEV | index) fit the 32-bit storage and widen back unchanged. */
long OpenEvent(unsigned long eventClass,long specification,long mode,
    BiosEventCallback callback);
long EnableEvent(long handle);
long TestEvent(long handle);
s32 FileRead(s32 descriptor,void *buffer,s32 count);
s32 FileWrite(s32 descriptor,const void *buffer,s32 count);
s32 FileOpen(const char *name,s32 mode);
s32 FileSeek(s32 descriptor,s32 offset,s32 whence);
/* PsyZ kernel: void DeliverEvent(unsigned desc, unsigned spec). */
void DeliverEvent(unsigned eventClass,unsigned specification);
#endif
