#ifndef RR_OVERLAY667_EVENTS_H
#define RR_OVERLAY667_EVENTS_H
#include "rr/bios_events.h"
#include "rr/driver_control.h"
#include "rr/overlay667_status.h"
extern RrBiosEventHandle ovl667_D_80118ED8,ovl667_D_80118EDC;
extern s32 ovl667_D_80118EE0,ovl667_D_80118EE8;
extern s32 LinkWriteHandle,LinkReadHandle;
extern const char ovl667_D_8011487C[];
/* The callback wraps its counter modulo2^32 and clears the pending word. */
s32 ovl667_func_80114D10(void);
/* Register two events, open sio: twice, then enable freshly read event handles.
 * Service boundaries may invoke the registered callback and mutate state. */
s32 ovl667_func_80114988(void);
void ovl667_func_80114950(void);
#endif
