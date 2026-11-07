/* CC1_FLAGS: -fno-strength-reduce */

#include "rr/menu_transition.h"

/* legacy typing of shared globals in this part of the unit */
#define SetDispMask ((int (*)())SetDispMask)

#define func_8005CA88 ((int (*)())func_8005CA88)

#define D_800F4D70 (*(int (*))&D_800F4D70)

#define D_800F4C6A (*(short (*))&D_800F4C6A)

#undef SetDispMask

#undef func_8005CA88

#undef D_800F4D70

#undef D_800F4C6A

extern s32 ScreenFrame;

void ResetScreenCounters(void) {
    D_800F4D7C = 0;
    D_800F3A2C = 0;
    ScreenFrame = 0;
}
