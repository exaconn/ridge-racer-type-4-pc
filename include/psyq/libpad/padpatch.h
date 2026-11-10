#ifndef RR_PSYQ_LIBPAD_PADPATCH_H
#define RR_PSYQ_LIBPAD_PADPATCH_H

#include "types.h"

s32 _padSetActAlign(void *channel, u8 *layout);
void _padSendActInfoCmd(void *channel);
s32 _padRecvActInfoReply(void *channel);
void _padActAlignUpdate(void *channel);
s32 _padActAlignComplete(void *channel);

#endif /* RR_PSYQ_LIBPAD_PADPATCH_H */
