#ifndef RR_GAME_SYSTEM_PAD3_H
#define RR_GAME_SYSTEM_PAD3_H

#include "types.h"

void PadRumbleStop(int i);
void PadSetRumbleParams(int i, short a, char b);
void PadRumbleStart(int i);
void PadSetRumbleFlags(int i, int flags);
void PadUpdateRumble(void);
void PadPollPort(int a0);


#endif /* RR_GAME_SYSTEM_PAD3_H */
