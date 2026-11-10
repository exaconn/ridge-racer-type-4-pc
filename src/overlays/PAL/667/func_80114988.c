#include "rr/overlay667_events.h"

s32 ovl667_open_sio_events(void) {
    ovl667_sio_read_event = OpenEvent(0xF000000B, 0x400, 0x2000, 0);
    ovl667_sio_error_event = OpenEvent(0xF000000B, 0x8000, 0x1000, ovl667_on_sio_error_event);
    LinkWriteHandle = FileOpen(ovl667_sio_device_name, 2);
    LinkReadHandle = FileOpen(ovl667_sio_device_name, 0x8001);
    EnableEvent(ovl667_sio_error_event);
    EnableEvent(ovl667_sio_read_event);
    return 0;
}
