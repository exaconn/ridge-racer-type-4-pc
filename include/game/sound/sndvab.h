#ifndef RR_GAME_SOUND_SNDVAB_H
#define RR_GAME_SOUND_SNDVAB_H

#include "types.h"
#include "rr/vab_data.h"

void ReverbReset(void);
void SetReverbDepthBoth(s32 arg0);
void PlaySfxVolPan(int a0, s16 a1, s16 a2, int a3);
void PlaySfxBank2(int);
void SoundQueueClear(void);
void LoadVab(const VabData *data, s32 mode, s32 bankIndex);


void PlaySfx(s32 sound);

#endif /* RR_GAME_SOUND_SNDVAB_H */
