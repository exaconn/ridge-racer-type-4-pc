#include "rr/audio_pan.h"

/* legacy typing of shared globals in this part of the unit */
#define func_8008AEC4 ((short (*)())func_8008AEC4)

#define D_800F4DB8 (*(short (*)[])&D_800F4DB8)

#undef func_8008AEC4

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x2894]; s32 a, b, c, d; } (*))&D_800F4DB8)

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x2894]; s32 a, b, c; } (*))&D_800F4DB8)

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x19D0]; s32 a; char p1[4]; s32 b, c; char p2[8]; s16 arr[8]; } (*))&D_800F4DB8)

#undef D_800F4DB8

void func_8004F218(int a0, s16 a1, s16 a2, int a3) {
    func_8004EF04(a0, a1, a2, 0x1E00, a3);
}
