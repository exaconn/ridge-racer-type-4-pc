#ifndef RR_GAME_SOUND_SNDOWN_H
#define RR_GAME_SOUND_SNDOWN_H

#include "types.h"
#include "rr/audio_bank_setup_types.h"

void SlotSetupBank(AudioBankSetup *input);
void ResetPlayerVoices(void);
void SetPlayerVoice(int a0, int a1, int a2, int a3);

#endif /* RR_GAME_SOUND_SNDOWN_H */
