#ifndef RR_CHANNEL_RESET_H
#define RR_CHANNEL_RESET_H
#include "rr/channels.h"

/* Lock value1 rejects the call without mutation. Otherwise a valid unsigned
 * low16 voice (0..23) selects fast reset only when all four signed low16 fields
 * match and row.value00 == 255. Every other valid case publishes the voice and
 * takes ordinary reset. Both callbacks run under lock1. Afterwards the original
 * voice's halfwords at1E/2A and the lock are cleared, regardless of callback
 * changes to the global selector. Invalid voices clear the lock and return-1. */
s32 func_8008564C(s32 voice, s32 bank, s32 program, s32 instrument, s32 group);
#endif
