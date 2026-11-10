/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
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
    GpuTimeoutDeadline = VSync(-1) + 0xF0U;
    GpuAlarmCount = 0;
    while ((*DMA2_CHCR & 0x1000000) || (*GPU_STATUS & 0x4000000) == 0) {
        if (get_alarm()) return -1;
    }
    DMACallback(2, GPU_DmaDoneRequeue);
    GpuFuncTable->download1C(rect, dst);
    return 0;
}

s32 MoveImage2(GpuRectangle *rect, s32 x, s32 y) {
    GpuMovePayload12 *payload;
    LibGraphicsDriverPrefix *driver;
    u32 position;
    checkRECT(MoveImageNameStr, rect);
    GpuTimeoutDeadline = VSync(-1) + 0xF0U;
    GpuAlarmCount = 0;
    while ((*DMA2_CHCR & 0x1000000) || (*GPU_STATUS & 0x4000000) == 0) {
        if (get_alarm()) return -1;
    }
    DMACallback(2, GPU_DmaDoneRequeue);
    if (rect->rect.w == 0 || rect->rect.h == 0) return -1;
    payload = &GpuMovePayload;
    position = rect->words[0];
    driver = GpuFuncTable;
    payload->destination = (y * 0x10000U) | (x & 0xFFFF);
    payload->position = position;
    payload->size = rect->words[1];
    driver->draw_ordering_table((unsigned long)payload - 8);
    return 0;
}
