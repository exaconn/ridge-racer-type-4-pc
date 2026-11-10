#ifndef RR_OVERLAY662_TIME_H
#define RR_OVERLAY662_TIME_H
#include "types.h"

extern u8 ovl662_record_time_str_buf[6];
/* Input uses 3000 units per second. Negative values produce --:--.
 * The existing separator and terminator are preserved; minutes are not clamped. */
u8 *ovl662_format_record_time(s32 value);
#endif
