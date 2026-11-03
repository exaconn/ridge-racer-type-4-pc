/* CC1_FLAGS: -mno-split-addresses */
#include "rr/gpu.h"
#include "rr/library.h"
#include "rr/timing.h"
#include "psyq/libgpu/gpuenv.h"

extern RrLibGraphicsDriverPrefix *D_800ABAB8;

int func_80095664();
s32 func_80095750(RrGpuRectangle *rect,s32 x,s32 y);

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((int *)D_800ABAB8)
#define func_80092DB8 ((int (*)())func_80092DB8)
#define func_8008B154 ((int (*)())func_8008B154)
#define func_8008B3C4 ((int (*)())func_8008B3C4)
#define func_80095394 ((int (*)())func_80095394)
#define func_80095990 ((int (*)())func_80095990)

int func_80095664(a0, a1)
int a0;
int a1;
{
    func_80092DB8(D_80011EF0, a0);
    D_800ABBFC = func_8008B154(-1) + 0xF0;
    D_800ABC00 = 0;
    while ((*D_800ABBD4 & 0x1000000) || (*D_800ABBC8 & 0x4000000) == 0) {
        if (func_80095394()) return -1;
    }
    func_8008B3C4(2, func_80095990);
    ((int (*)())D_800ABAB8[7])(a0, a1);
    return 0;
}

#undef D_800ABAB8
#undef func_80092DB8
#undef func_8008B154
#undef func_8008B3C4
#undef func_80095394
#undef func_80095990

s32 func_80095750(RrGpuRectangle *rect,s32 x,s32 y) {
    /* ASM_BARRIER: Keep each DMA status read in the original polling temporary. */
    register u32 status __asm__("$2");
    func_80092DB8(D_80011EFC,rect);
    D_800ABBFC=(u32)func_8008B154(-1)+240;
    D_800ABC00=0;
    status=*D_800ABBD4;
    goto test;
wait:
    if(func_80095394()) return -1;
    status=*D_800ABBD4;
test:
    if(status&0x01000000) goto wait;
    if(!(*D_800ABBC8&0x04000000)) goto wait;
    {
        func_8008B3C4(2,func_80095990);
        if(rect->rect.w) {
            if(rect->rect.h) goto build;
        }
        return -1;
    build:
        {
            RrGpuMovePayload *payload;
            /* ASM_BARRIER: Form destination Y in the original height-test delay slot. */
            register u32 destination __asm__("$2")=(u32)y<<16;
            u32 position=rect->words[0];
            /* ASM_BARRIER: Retain the captured driver until packet publication completes. */
            register RrLibGraphicsDriverPrefix *driver __asm__("$3")=D_800ABAB8;
            destination|=(u32)x&65535;
            /* ASM_BARRIER: Capture the source position and driver before forming the packet address. */
            __asm__("" : : "r"(position),"r"(driver) : "$4");
            payload=&D_800ABB60;
            payload->destination=destination;
            payload->position=position;
            payload->size=rect->words[1];
            driver->draw_ordering_table((unsigned long)payload-8);
        }
        return 0;
    }
}
