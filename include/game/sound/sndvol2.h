#ifndef RR_GAME_SOUND_SNDVOL2_H
#define RR_GAME_SOUND_SNDVOL2_H

#include "rr/audio_state_types.h"

enum { RR_PLAYER_VOICE_MODE_COUNT = 3 };
extern const AudioPlayerVoiceParameters PlayerVoiceParamTable[RR_PLAYER_VOICE_MODE_COUNT];

/* Modes0..3 use the silent state and three parameter records. View mode is
 * 0 or1; selected slots/pans stay in0..3/0..127. */
void UpdatePlayerVoices(void);

#endif /* RR_GAME_SOUND_SNDVOL2_H */
