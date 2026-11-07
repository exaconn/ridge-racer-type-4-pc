#include "rr/audio_loop.h"
void SetVoiceVolumeScaled(s32 a0, int a1, int a2) {
    int v = VoiceVolumeScale;
    SsUtSetVVol((s16)a0, (s16)((s32)((u32)a1 * (u32)v) >> 7), (s16)((s32)((u32)a2 * (u32)v) >> 7));
}
