#ifndef RR_PSYQ_LIBPAD_PADCTL_H
#define RR_PSYQ_LIBPAD_PADCTL_H

#include "types.h"

void PadStartCom();
s32 PadGetState(s32 selector);
s32 PadInfoMode(s32 selector, s32 query, s32 index);
s32 PadInfoAct(s32 selector, s32 index, s32 query);
s32 PadSetActAlign(s32 selector, u8 *layout);
void PadSetAct(s32 selector, u8 *outputs, s32 count);

#endif /* RR_PSYQ_LIBPAD_PADCTL_H */
