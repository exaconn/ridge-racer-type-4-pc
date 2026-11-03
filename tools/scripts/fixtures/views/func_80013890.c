#include "rr/renderer_setup.h"

#define B (*(int *)&D_800ACC18)

/* legacy typing of shared globals in this part of the unit */
#define func_80070108 ((void (*)(void *, s32))func_80070108)

#define D_800ACC18 ((Resource *)D_800ACC18)

#define D_800F3A08 (*(void *(*)[])&D_800F3A08)

#undef func_80070108

#undef D_800ACC18

#undef D_800F3A08

/* legacy typing of shared globals in this part of the unit */
#define func_80092D50 ((int (*)())func_80092D50)

#define func_80092FFC ((int (*)())func_80092FFC)

#undef func_80092D50

#undef func_80092FFC

/* legacy typing of shared globals in this part of the unit */
#define D_8010BF60 ((short *)D_8010BF60)

#undef D_8010BF60

void func_80013890(s32 shift) {
    /* ASM_BARRIER: reuse the incoming argument register for the scratch base. */
    register RrRendererScratch *scratch __asm__("$4");
    *(s32*)0x1F800060U=shift;
    scratch=(RrRendererScratch*)0x1F800000U;
    scratch->renderFlags64=D_800AD568;
    D_800F3A20.b=128;
    D_800F3A20.g=128;
    D_800F3A20.r=128;
    D_800F3A20.code=0x2C;
    D_800F3A18.b=255;
    D_800F3A18.g=255;
    D_800F3A18.r=255;
    D_800F3A18.code=0x3C;
    scratch->viewport6C.rect.w=320;
    scratch->viewport6C.rect.x=0;
    scratch->viewport6C.rect.y=0;
    scratch->viewport6C.rect.h=256;
}
