#include "rr/renderer_setup.h"

void SetupRenderer(s32 shift) {
    /* ASM_BARRIER: reuse the incoming argument register for the scratch base. */
    register RendererScratch *scratch __asm__("$4");
    *(s32*)0x1F800060U=shift;
    scratch=(RendererScratch*)0x1F800000U;
    scratch->renderFlags64=MirrorRenderFlag;
    DefaultScreenClearColor.b=128;
    DefaultScreenClearColor.g=128;
    DefaultScreenClearColor.r=128;
    DefaultScreenClearColor.code=0x2C;
    LoadingScreenClearColor.b=255;
    LoadingScreenClearColor.g=255;
    LoadingScreenClearColor.r=255;
    LoadingScreenClearColor.code=0x3C;
    scratch->viewport6C.rect.w=320;
    scratch->viewport6C.rect.x=0;
    scratch->viewport6C.rect.y=0;
    scratch->viewport6C.rect.h=256;
}
