#ifndef RR_CHANNEL_RESET_H
#define RR_CHANNEL_RESET_H
#include "rr/channels.h"

/* Lock value1 rejects the call without mutation. Otherwise a valid unsigned
 * low16 voice (0..23) selects fast reset only when all four signed low16 fields
 * match and row.value00 == 255. Every other valid case publishes the voice and
 * takes ordinary reset. Both callbacks run under lock1. Afterwards the original
 * voice's halfwords at1E/2A and the lock are cleared, regardless of callback
 * changes to the global selector. Invalid voices clear the lock and return-1. */
s16 SsUtKeyOff(s16 voice, s16 bank, s16 program, s16 instrument, s16 group);
#endif
