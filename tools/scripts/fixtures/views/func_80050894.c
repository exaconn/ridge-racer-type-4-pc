#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define SoundState (*(RrAudioOwner (*))&SoundState)

#undef SoundState

struct B { char b[0x5BC]; };

struct E { struct B x; char pad[1568 - 0x5BC]; };

extern struct E D_800F4E2C[];

void SlotLoadCurves(struct B *a0, int a1) {
    D_800F4E2C[a1].x = *a0;
}
