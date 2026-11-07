#include "types.h"
extern int SsSetSerialVol();
extern u8 D_800AD558;
extern struct { char pad[0x26C4]; u16 v; } SoundState;
void CdAudioSetVolume(u16 a0) {
    s16 s;
    if (SoundState.v != a0) {
        SoundState.v = (u8)a0;
        s = (SoundState.v * D_800AD558) / 16;
        SsSetSerialVol(0, s, s);
    }
}
