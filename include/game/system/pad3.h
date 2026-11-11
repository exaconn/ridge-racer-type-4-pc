#ifndef RR_GAME_SYSTEM_PAD3_H
#define RR_GAME_SYSTEM_PAD3_H

#include "types.h"

void PadRumbleStop(int i);
/* Full call words are narrowed only when stored in the controller record. */
void PadSetRumbleParams(s32 channel, s32 direction, s32 level);
void PadRumbleStart(int i);
void PadSetRumbleFlags(int i, int flags);
void PadUpdateRumble(void);
void PadPollPort(int a0);


#endif /* RR_GAME_SYSTEM_PAD3_H */
