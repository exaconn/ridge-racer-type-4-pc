#ifndef RR_GAME_SOUND_CDMAIN_H
#define RR_GAME_SOUND_CDMAIN_H

#include "types.h"

void CdAudioReadyCallback(u8 a0);
void ResetSeqFadeState(void);
void CdAudioUpdate(void);
void SeqSetVolume(s16 a0);
void SeqFadeVolume(int a0, int a1);


#endif /* RR_GAME_SOUND_CDMAIN_H */
