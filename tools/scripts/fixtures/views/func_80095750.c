/* CC1_FLAGS: -mno-split-addresses */

#include "rr/gpu.h"

#include "rr/library.h"

#include "rr/timing.h"

/* legacy typing of shared globals in this part of the unit */
#define GpuFuncTable ((int *)GpuFuncTable)

#define checkRECT ((int (*)())checkRECT)

#define VSync ((int (*)())VSync)

#define DMACallback ((int (*)())DMACallback)

#define get_alarm ((int (*)())get_alarm)

#define GPU_DmaDoneRequeue ((int (*)())GPU_DmaDoneRequeue)

#undef GpuFuncTable

#undef checkRECT

#undef VSync

#undef DMACallback

#undef get_alarm

#undef GPU_DmaDoneRequeue

extern char D_80011EFC[];

extern RrLibGraphicsDriverPrefix *GpuFuncTable;

s32 MoveImage2(RrGpuRectangle *rect,s32 x,s32 y) {
    /* ASM_BARRIER: Keep each DMA status read in the original polling temporary. */
    register u32 status __asm__("$2");
    checkRECT(D_80011EFC,rect);
    D_800ABBFC=(u32)VSync(-1)+240;
    GpuAlarmCount=0;
    status=*DMA2_CHCR;
    goto test;
wait:
    if(get_alarm()) return -1;
    status=*DMA2_CHCR;
test:
    if(status&0x01000000) goto wait;
    if(!(*GPU_STATUS&0x04000000)) goto wait;
    {
        DMACallback(2,GPU_DmaDoneRequeue);
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
            register RrLibGraphicsDriverPrefix *driver __asm__("$3")=GpuFuncTable;
            destination|=(u32)x&65535;
            /* ASM_BARRIER: Capture the source position and driver before forming the packet address. */
            __asm__("" : : "r"(position),"r"(driver) : "$4");
            payload=&GpuMovePayload;
            payload->destination=destination;
            payload->position=position;
            payload->size=rect->words[1];
            driver->draw_ordering_table((unsigned long)payload-8);
        }
        return 0;
    }
}
