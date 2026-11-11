#include "rr/library.h"
#include "rr/gpu.h"
#include "rr/gpu_reset.h"
extern char LoadImage2NameStr[];
int LoadImage2(GpuRectangle *a0, const u32 *a1) {
    checkRECT(LoadImage2NameStr, a0);
    GpuTimeoutDeadline = VSync(-1) + 0xF0;
    GpuAlarmCount = 0;
    while ((*DMA2_CHCR & 0x01000000) || !(*GPU_STATUS & 0x04000000)) {
        if (get_alarm() != 0) return -1;
    }
    DMACallback(2, GPU_DmaDoneRequeue);
    GpuFuncTable->upload20(a0, a1);
    return 0;
}
