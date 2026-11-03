#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(RrAudioOwner (*))&D_800F4DB8)

#undef D_800F4DB8

struct B { char b[0x5BC]; };

struct E { struct B x; char pad[1568 - 0x5BC]; };

extern struct E D_800F4E2C[];

void func_80050894(struct B *a0, int a1) {
    D_800F4E2C[a1].x = *a0;
}
