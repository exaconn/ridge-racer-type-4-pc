#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define SoundState (*(struct E (*)[])&SoundState)

#undef SoundState

/* legacy typing of shared globals in this part of the unit */
#define SoundState (*(struct { char pad[0x18]; s16 a, b, c, d, e, f; } (*))&SoundState)

#undef SoundState

/* legacy typing of shared globals in this part of the unit */
#define SoundState (*(struct { char pad[0x18]; unsigned short a; short b; short c; unsigned short d; unsigned short e; } (*))&SoundState)

#undef SoundState

extern struct {
    char pad00[0x24];
    short h24[0x1A];
    char pad0[0x1A08-0x24-0x34];
    int f1a08;
    int a1a0c[7];
    char pad2[0x1A2C - 0x1A28];
    int f1a2c;
    int f1a30;
    char pad3[0x2864 - 0x1A34];
    int f2864;
    int f2868;
} SoundState;

extern void CdAudioFade(int, short);

void SoundSetMasterVolume(short arg0, short arg1) {
    if (arg1 >= 2) {
        SoundState.b = arg1;
        SoundState.c = 0;
        SoundState.d = arg0;
        SoundState.e = SoundState.a;
    } else {
        SoundState.a = arg0;
        SoundState.b = 0;
        SoundState.c = 0;
        SoundState.d = 0;
        SoundState.e = 0;
        if (arg0 == 0) {
            SoundStopAll();
        }
    }
    CdAudioFade(arg0 & 0xFFFF, arg1);
}
