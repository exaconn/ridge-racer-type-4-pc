#include "types.h"

#define G ((Scratch_8006EF8C *)0x1F800000)

/* legacy typing of shared globals in this part of the unit */
#define D_800ABED0 (*(M2C_UNK (*))&D_800ABED0)

#undef D_800ABED0

/* legacy typing of shared globals in this part of the unit */
#define func_80060C70 ((int (*)())func_80060C70)

#define func_8006E834 ((int (*)())func_8006E834)

#define func_80071348 ((int (*)())func_80071348)

#undef func_80060C70

#undef func_8006E834

#undef func_80071348

/* legacy typing of shared globals in this part of the unit */
#define func_8006E834 ((int (*)())func_8006E834)

#define func_80060C70 ((int (*)())func_80060C70)

#undef func_8006E834

#undef func_80060C70

extern int SetTransMatrix();

extern int SetRotMatrix();

extern int ApplyMatrix();

void SetGteObjectTransform(short *a0, short *a1, int *a2) {
    short *g = (short *)0x1F800000;
    a0[0] = a1[0] - g[4];
    a0[1] = a1[2] - g[6];
    a0[2] = a1[4] - g[8];
    ApplyMatrix(0x1F800028, a0, a0 + 4);
    *(int *)(a0 + 0x16) = *(int *)(a0 + 4);
    *(int *)(a0 + 0x18) = *(int *)(a0 + 6);
    *(int *)(a0 + 0x1A) = *(int *)(a0 + 8);
    SetRotMatrix(a2);
    SetTransMatrix(a0 + 12);
}
