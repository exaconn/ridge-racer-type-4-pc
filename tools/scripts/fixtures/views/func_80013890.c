#include "rr/renderer_setup.h"

#define B (*(int *)&LoadCursor)

/* legacy typing of shared globals in this part of the unit */
#define RegisterModelBank ((void (*)(void *, s32))RegisterModelBank)

#define LoadCursor ((Resource *)LoadCursor)

#define CarBoundsTable (*(void *(*)[])&CarBoundsTable)

#undef RegisterModelBank

#undef LoadCursor

#undef CarBoundsTable

/* legacy typing of shared globals in this part of the unit */
#define DrawSync ((int (*)())DrawSync)

#define LoadImage ((int (*)())LoadImage)

#undef DrawSync

#undef LoadImage

/* legacy typing of shared globals in this part of the unit */
#define D_8010BF60 ((short *)D_8010BF60)

#undef D_8010BF60

void SetupRenderer(s32 shift) {
    /* ASM_BARRIER: reuse the incoming argument register for the scratch base. */
    register RendererScratch *scratch __asm__("$4");
    *(s32*)0x1F800060U=shift;
    scratch=(RendererScratch*)0x1F800000U;
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
