#ifndef RR_OVERLAY667_EVENTS_H
#define RR_OVERLAY667_EVENTS_H
#include "rr/bios_events.h"
#include "rr/driver_control.h"
#include "rr/overlay667_status.h"
extern BiosEventHandle ovl667_sio_read_event,ovl667_sio_error_event;
extern s32 ovl667_sio_error_count,ovl667_read_pending_flag;
extern s32 LinkWriteHandle,LinkReadHandle;
extern const char ovl667_sio_device_name[8];
/* Default names "PLAYER 4".."PLAYER 1" that follow the device name. */
extern char ovl667_player_name_text[4][12];
/* The callback wraps its counter modulo2^32 and clears the pending word. */
s32 ovl667_on_sio_error_event(void);
/* Register two events, open sio: twice, then enable freshly read event handles.
 * Service boundaries may invoke the registered callback and mutate state. */
s32 ovl667_open_sio_events(void);
void ovl667_reset_sio_ports(void);
#endif
