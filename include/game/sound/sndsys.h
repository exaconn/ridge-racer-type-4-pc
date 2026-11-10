#ifndef RR_GAME_SOUND_SNDSYS_H
#define RR_GAME_SOUND_SNDSYS_H

#include "types.h"
#include "rr/audio_bank_setup_types.h"

void ComSlotSetupBank(AudioBankSetup *input);
/* Copy one non-overlapping resource payload; source byte alignment is unrestricted. */
void SlotLoadCurves(const void *data, s32 slot);
int SlotSetProgram(int a0, int a1, int a2, int a3);


#endif /* RR_GAME_SOUND_SNDSYS_H */
