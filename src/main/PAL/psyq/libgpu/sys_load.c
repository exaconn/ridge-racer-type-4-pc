/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
extern int VSync();
extern void checkRECT();
extern void GPU_DmaDoneRequeue();
extern int get_alarm();
extern char LoadImage2NameStr[];
extern char *GpuFuncTable;
extern volatile int *GPU_STATUS;
extern volatile int *DMA2_CHCR;
extern int GpuTimeoutDeadline;
extern int GpuAlarmCount;
int LoadImage2(int a0, int a1) {
    checkRECT(LoadImage2NameStr, a0);
    GpuTimeoutDeadline = VSync(-1) + 0xF0;
    GpuAlarmCount = 0;
    while ((*DMA2_CHCR & 0x01000000) || !(*GPU_STATUS & 0x04000000)) {
        if (get_alarm() != 0) return -1;
    }
    DMACallback(2, GPU_DmaDoneRequeue);
    (*(void (**)())(GpuFuncTable + 0x20))(a0, a1);
    return 0;
}
