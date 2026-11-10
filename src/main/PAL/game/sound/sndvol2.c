#include "types.h"
#include "rr/audio_loop.h"
#include "game/sound/sndvol2.h"
#include "rr/channels.h"
#include "rr/channel_pitch.h"
#include "rr/audio_pan.h"

typedef struct {
    s32 a;
    s32 b;
    s32 c;
} TableEntry;

extern AudioPlayerVoice1C D_800F6714[2];
extern const TableEntry PlayerVoiceParamTable[];

void UpdatePlayerVoices(void) {
    int i;
    AudioPlayerVoice1C *s0;
    AudioSlot620 *s2;
    int s6, s7, fp;
    int a1, a2, v0;

    for (i = 0; i < 2; i++) {
        s0 = &D_800F6714[i];
        if (s0->prev0C != s0->mode08) {
            if (s0->mode08 == 0) {
                SsUtKeyOffV((s16)s0->voice04);
            } else if (s0->mode08 >= 0) {
                if (s0->mode08 < 4) {
                    SsUtKeyOnV((s16)s0->voice04, (s16)s0->bank00, (s16)PlayerVoiceParamTable[s0->mode08 - 1].a, 0, 0x3C, 0, 0, 0);
                }
            }
            s0->prev0C = s0->mode08;
        }
        if (s0->mode08 != 0) {
            s6 = PlayerVoiceParamTable[s0->mode08 - 1].a;
            s7 = s0->pitch10 >> 7;
            fp = s0->pitch10 & 0x7F;
            a1 = (s0->vol14 * PlayerVoiceParamTable[s0->mode08 - 1].c) / 128;
            switch (SoundState.submode18C2) {
            case 0:
                switch (i) {
                case 0:
                    s2 = &SoundState.slots38[0];
                    break;
                case 1:
                    s2 = &SoundState.slots38[SoundState.playerVoices195C[1].slot18];
                    break;
                }
                break;
            case 1:
                s2 = &SoundState.slots38[i];
                break;
            }
            a2 = (PanGainTable[s2->pan24] * a1) / 16129;
            a1 = (PanGainTable[(s16)(0x7F - s2->pan24)] * a1) / 16129;
            a2 = a2 * s2->levels20.halves.gain;
            if (a2 < 0) a2 += 0x7F;
            a1 = a1 * s2->levels20.halves.gain;
            a2 >>= 7;
            if (a1 < 0) a1 += 0x7F;
            a1 >>= 7;
            if (Options.audio.mono32 != 0) {
                a2 = (a1 + a2) / 2;
                a1 = a2;
            }
            v0 = a2 * SoundState.gain18;
            if (v0 < 0) v0 += 0x7F;
            a1 = a1 * SoundState.gain18;
            a2 = v0 >> 7;
            if (a1 < 0) a1 += 0x7F;
            a1 >>= 7;
            v0 = a2 * Options.audio.volume31;
            if (v0 < 0) v0 += 0xF;
            a1 = a1 * Options.audio.volume31;
            a2 = v0 >> 4;
            if (a1 < 0) a1 += 0xF;
            a1 >>= 4;
            if (SoundState.muted04 != 0) {
                a2 = 0;
                a1 = 0;
            }
            SetVoiceVolumeScaled(s0->voice04, a1, a2);
            SsUtChangePitch((s16)s0->voice04, (s16)s0->bank00, (s16)s6, 0x3C, 0, (s16)s7, fp);
        }
    }
}
