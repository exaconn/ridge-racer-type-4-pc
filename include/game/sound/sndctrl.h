#ifndef RR_GAME_SOUND_SNDCTRL_H
#define RR_GAME_SOUND_SNDCTRL_H

#include "types.h"

void SoundReset(int a0, int a1);
void SoundReverbOffAll(void);
void SoundPause(void);
void SoundResume(void);
int SoundStopAll();
void SoundMasterVolumeReset(void);

#endif /* RR_GAME_SOUND_SNDCTRL_H */
