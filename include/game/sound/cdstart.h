#ifndef RR_GAME_SOUND_CDSTART_H
#define RR_GAME_SOUND_CDSTART_H

#include "types.h"

void CdAudioBeginPlay(int a0);
void CdAudioMute(void);
void CdAudioSetVolume(u16 a0);
void CdAudioSetVolumeRaw(s32 arg0);
void CdAudioFade(s32 a0, s32 a1);
void CdAudioRequestPause(void);
void CdAudioSetFilter(int a0);
int CdAudioFilterPending(void);
void CdAudioFilterStep(void);

#endif /* RR_GAME_SOUND_CDSTART_H */
