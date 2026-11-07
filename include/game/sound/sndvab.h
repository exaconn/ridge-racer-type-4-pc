#ifndef RR_GAME_SOUND_SNDVAB_H
#define RR_GAME_SOUND_SNDVAB_H

#include "types.h"

void ReverbReset(void);
void SetReverbDepthBoth(s32 arg0);
void PlaySfxVolPan(int a0, s16 a1, s16 a2, int a3);
void PlaySfxBank2(int);
void SoundQueueClear(void);

#endif /* RR_GAME_SOUND_SNDVAB_H */
