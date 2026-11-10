#include "rr/overlay667_events.h"

s32 ovl667_on_sio_error_event(void) {
    union {u32 bits;s32 value;} count;
    count.bits=ovl667_sio_error_count+1U;
    ovl667_sio_error_count=count.value;
    ovl667_read_pending_flag = 0;
    return 0;
}
