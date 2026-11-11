#include "types.h"
#include "rr/audio_loop.h"
#include "game/sound/sndvol2.h"
#include "rr/channels.h"
#include "rr/channel_pitch.h"
#include "rr/audio_pan.h"

void UpdatePlayerVoices(void) {
    int playerIndex;
    AudioPlayerVoice1C *voice;
    AudioSlot620 *slot;
    int program, pitchCoarse, pitchFine;
    int leftVolume, rightVolume, scaledRight;

    for (playerIndex = 0; playerIndex < 2; playerIndex++) {
        voice = &SoundState.playerVoices195C[playerIndex];
        if (voice->prev0C != voice->mode08) {
            if (voice->mode08 == 0) {
                SsUtKeyOffV((s16)voice->voice04);
            } else if (voice->mode08 >= 0) {
                if (voice->mode08 <= RR_PLAYER_VOICE_MODE_COUNT) {
                    SsUtKeyOnV((s16)voice->voice04, (s16)voice->bank00, (s16)PlayerVoiceParamTable[voice->mode08 - 1].program00, 0, 0x3C, 0, 0, 0);
                }
            }
            voice->prev0C = voice->mode08;
        }
        if (voice->mode08 != 0) {
            program = PlayerVoiceParamTable[voice->mode08 - 1].program00;
            pitchCoarse = voice->pitch10 >> 7;
            pitchFine = voice->pitch10 & 0x7F;
            leftVolume = (voice->vol14 * PlayerVoiceParamTable[voice->mode08 - 1].gain08) / 128;
            switch (SoundState.submode18C2) {
            case 0:
                switch (playerIndex) {
                case 0:
                    slot = &SoundState.slots38[0];
                    break;
                case 1:
                    slot = &SoundState.slots38[SoundState.playerVoices195C[1].slot18];
                    break;
                }
                break;
            case 1:
                slot = &SoundState.slots38[playerIndex];
                break;
            }
            rightVolume = (PanGainTable[slot->pan24] * leftVolume) / 16129;
            leftVolume = (PanGainTable[(s16)(0x7F - slot->pan24)] * leftVolume) / 16129;
            rightVolume = rightVolume * slot->levels20.halves.gain;
            if (rightVolume < 0) rightVolume += 0x7F;
            leftVolume = leftVolume * slot->levels20.halves.gain;
            rightVolume >>= 7;
            if (leftVolume < 0) leftVolume += 0x7F;
            leftVolume >>= 7;
            if (Options.audio.mono32 != 0) {
                rightVolume = (leftVolume + rightVolume) / 2;
                leftVolume = rightVolume;
            }
            scaledRight = rightVolume * SoundState.gain18;
            if (scaledRight < 0) scaledRight += 0x7F;
            leftVolume = leftVolume * SoundState.gain18;
            rightVolume = scaledRight >> 7;
            if (leftVolume < 0) leftVolume += 0x7F;
            leftVolume >>= 7;
            scaledRight = rightVolume * Options.audio.volume31;
            if (scaledRight < 0) scaledRight += 0xF;
            leftVolume = leftVolume * Options.audio.volume31;
            rightVolume = scaledRight >> 4;
            if (leftVolume < 0) leftVolume += 0xF;
            leftVolume >>= 4;
            if (SoundState.muted04 != 0) {
                rightVolume = 0;
                leftVolume = 0;
            }
            SetVoiceVolumeScaled(voice->voice04, leftVolume, rightVolume);
            SsUtChangePitch((s16)voice->voice04, (s16)voice->bank00, (s16)program, 0x3C, 0, (s16)pitchCoarse, pitchFine);
        }
    }
}
