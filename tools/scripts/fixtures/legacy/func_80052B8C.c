#include "types.h"
extern int CdAudioSetVolume();
extern struct { char p0[0x2670]; s32 x2670; char p1[0x4]; s32 x2678; char p2[0x20]; s32 x269C; char p3[0x8]; s32 x26A8; char p4[0x4]; s32 x26B0; s32 x26B4; char p5[0xC]; u16 x26C4; s16 x26C6; s16 x26C8; u16 x26CA; u16 x26CC; u16 x26CE; } SoundState;
void CdAudioFade(u16 a0, int a1) {
    if (a1 >= 2) {
        u16 v = SoundState.x26C4;
        SoundState.x26C6 = a1;
        SoundState.x26C8 = 0;
        SoundState.x26CA = a0;
        SoundState.x26CC = v;
    } else {
        CdAudioSetVolume(a0);
        SoundState.x26C6 = 0;
        SoundState.x26C8 = 0;
        SoundState.x26CA = 0;
        SoundState.x26CC = 0;
        if (a0 == 0) {
            int r = SoundState.x269C;
            if (r != -1) {
                if (SoundState.x2670 != 1) {
                    SoundState.x2670 = 4;
                    SoundState.x2678 = 0;
                    SoundState.x26A8 = 0;
                    if (r == 2) CdAudioSetVolume(SoundState.x26CE);
                } else {
                    if (SoundState.x26B0 < 0x20) {
                        SoundState.x26B4 = 2;
                    } else {
                        SoundState.x2670 = 4;
                        SoundState.x2678 = 0;
                        SoundState.x26A8 = 0;
                    }
                }
            }
        }
    }
}
