#include "rr/audio_pan.h"

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

void PlaySfxVolPan(int a0, s16 a1, s16 a2, int a3) {
    TriggerSfx(a0, a1, a2, 0x1E00, a3);
}
