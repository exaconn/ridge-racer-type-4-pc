/* CC1_FLAGS: -mno-split-addresses */

#include "rr/gpu.h"

#include "rr/library.h"

#include "rr/timing.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800ABAB8 ((int *)D_800ABAB8)

#define checkRECT ((int (*)())checkRECT)

#define VSync ((int (*)())VSync)

#define DMACallback ((int (*)())DMACallback)

#define get_alarm ((int (*)())get_alarm)

#define func_80095990 ((int (*)())func_80095990)

#undef D_800ABAB8

#undef checkRECT

#undef VSync

#undef DMACallback

#undef get_alarm

#undef func_80095990

extern char D_80011EFC[];

extern RrLibGraphicsDriverPrefix *D_800ABAB8;

s32 MoveImage2(RrGpuRectangle *rect,s32 x,s32 y) {
    /* ASM_BARRIER: Keep each DMA status read in the original polling temporary. */
    register u32 status __asm__("$2");
    checkRECT(D_80011EFC,rect);
    D_800ABBFC=(u32)VSync(-1)+240;
    D_800ABC00=0;
    status=*D_800ABBD4;
    goto test;
wait:
    if(get_alarm()) return -1;
    status=*D_800ABBD4;
test:
    if(status&0x01000000) goto wait;
    if(!(*D_800ABBC8&0x04000000)) goto wait;
    {
        DMACallback(2,func_80095990);
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
