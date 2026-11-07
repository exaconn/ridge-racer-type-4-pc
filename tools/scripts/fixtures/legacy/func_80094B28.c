#include "rr/gpu.h"
void _cwc(unsigned long address) {
    *GPU_STATUS=0x04000002;
    *DMA2_MADR=(u32)address;
    *DMA2_BCR=0;
    *(u32 *)DMA2_CHCR=0x01000401;
}
