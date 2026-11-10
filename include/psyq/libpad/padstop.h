#ifndef RR_PSYQ_LIBPAD_PADSTOP_H
#define RR_PSYQ_LIBPAD_PADSTOP_H

#include "types.h"

/* Any bit in the high nibble of the low byte selects bank one.
 * Other selector bits do not affect this two-bank lookup. */
void *_padPortToChannel(s32 selector);

/* The two bank bases map to 0x10 and 0x20; other addresses map to 0xFF. */
s32 _padChannelToPort(void *channel);

#endif
