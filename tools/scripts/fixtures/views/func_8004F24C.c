#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define SsVabTransBody ((short (*)())SsVabTransBody)

#define SoundState (*(short (*)[])&SoundState)

#undef SsVabTransBody

#undef SoundState

/* legacy typing of shared globals in this part of the unit */
#define SoundState (*(struct { char pad[0x2894]; s32 a, b, c, d; } (*))&SoundState)

#undef SoundState

/* legacy typing of shared globals in this part of the unit */
#define SoundState (*(struct { char pad[0x2894]; s32 a, b, c; } (*))&SoundState)

#undef SoundState

/* legacy typing of shared globals in this part of the unit */
#define SoundState (*(struct { char pad[0x19D0]; s32 a; char p1[4]; s32 b, c; char p2[8]; s16 arr[8]; } (*))&SoundState)

#undef SoundState

void PlaySfx(int a0) {
    PlaySfxVolPan(a0, 0x7F, 0x40, -1);
}
