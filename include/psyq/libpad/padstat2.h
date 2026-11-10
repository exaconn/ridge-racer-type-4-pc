#ifndef RR_PSYQ_LIBPAD_PADSTAT2_H
#define RR_PSYQ_LIBPAD_PADSTAT2_H

#include "rr/channels.h"

void _padSetAct(void *channel, u8 *outputs, s32 count);
s32 _padRecvAtLoadInfo(void *channel);
s32 _padGetActSize(const PadChannel *channel);
s32 _padLoadActInfo(PadChannel *channel,u8 *arena);

#endif /* RR_PSYQ_LIBPAD_PADSTAT2_H */
