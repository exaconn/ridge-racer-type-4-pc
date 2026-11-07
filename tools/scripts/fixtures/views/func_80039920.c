#include "types.h"

#define P8(base, off) (*(unsigned char *)((char *)(base) + (off)))

#define IDX30(base, i) (*(unsigned char *)((char *)(base) + (i) * 0x24 + 0x30))

/* legacy typing of shared globals in this part of the unit */
#define PlayerCars (*(char (*)[])&PlayerCars)

#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)

#undef PlayerCars

#undef D_800F4D8A

/* legacy typing of shared globals in this part of the unit */
#define PlayerCars (*(char (*)[])&PlayerCars)

#define D_800AD4D8 (*(char (*)[])&D_800AD4D8)

#define D_800F2AB0 ((W7C0_S *)D_800F2AB0)

#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)

#define UpdateScreenFade ((void (*)())UpdateScreenFade)

#define func_8002AF8C ((void (*)())func_8002AF8C)

#define StepMenuValue ((void (*)())StepMenuValue)

#define func_8002CC88 ((void (*)())func_8002CC88)

#define func_8002E5A0 ((void (*)())func_8002E5A0)

#define func_80033A74 ((void (*)())func_80033A74)

#define ResetScreenCounters ((void (*)())ResetScreenCounters)

#define func_8003FBA8 ((void (*)())func_8003FBA8)

#define SetupViewport ((void (*)())SetupViewport)

#undef PlayerCars

#undef D_800AD4D8

#undef D_800F2AB0

#undef D_800F4D8A

#undef UpdateScreenFade

#undef func_8002AF8C

#undef StepMenuValue

#undef func_8002CC88

#undef func_8002E5A0

#undef func_80033A74

#undef ResetScreenCounters

#undef func_8003FBA8

#undef SetupViewport

extern unsigned char D_8009F810[];

extern unsigned char D_800AD4D8[];

extern unsigned char D_800AD4EB;

extern unsigned char D_800F3920[];

void func_80039920(void) {
    int i = D_800AD4D8[0x1C] * 2 + (D_800AD4D8[0x1D] >> 1);
    int v = D_8009F810[i];
    if (v < 0) {
        v = 0;
    } else if (v >= 9) {
        v = 8;
    }
    D_800AD4EB = D_800F3920[v + 0x60];
}
