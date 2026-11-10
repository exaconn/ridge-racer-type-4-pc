#ifndef RR_GAME_RACE_OBJSIM_H
#define RR_GAME_RACE_OBJSIM_H

#include "types.h"

extern short D_800AC0E6;
extern s16 D_800AC1A2;

s32 InterpolateObjectKeyframe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void UpdateTrackObjects();

#endif /* RR_GAME_RACE_OBJSIM_H */
