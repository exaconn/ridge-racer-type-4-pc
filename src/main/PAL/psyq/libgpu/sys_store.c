/* CC1_FLAGS: -mno-split-addresses */
#include "rr/gpu.h"
#include "rr/library.h"
#include "rr/timing.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/gpu_reset.h"

int StoreImage2();
s32 MoveImage2(GpuRectangle *rect,s32 x,s32 y);

int StoreImage2(GpuRectangle *rect, void *dst)
{
    checkRECT(StrStoreImage, rect);
    GpuTimeoutDeadline = VSync(-1) + 0xF0;
    GpuAlarmCount = 0;
    while ((*DMA2_CHCR & 0x1000000) || (*GPU_STATUS & 0x4000000) == 0) {
        if (get_alarm()) return -1;
    }
    DMACallback(2, GPU_DmaDoneRequeue);
    GpuFuncTable->download1C(rect, dst);
    return 0;
}

s32 MoveImage2(GpuRectangle *rect,s32 x,s32 y) {
    /* ASM_BARRIER: Keep each DMA status read in the original polling temporary. */
    register u32 status __asm__("$2");
    checkRECT(MoveImageNameStr,rect);
    GpuTimeoutDeadline=(u32)VSync(-1)+240;
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
            GpuMovePayload12 *payload;
            /* ASM_BARRIER: Form destination Y in the original height-test delay slot. */
            register u32 destination __asm__("$2")=y * 0x10000U;
            u32 position=rect->words[0];
            /* ASM_BARRIER: Retain the captured driver until packet publication completes. */
            register LibGraphicsDriverPrefix *driver __asm__("$3")=GpuFuncTable;
            destination|=x&65535;
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
