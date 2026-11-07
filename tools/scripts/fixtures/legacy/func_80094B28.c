#include "rr/gpu.h"
void _cwc(unsigned long address) {
    *D_800ABBC8=0x04000002;
    *D_800ABBCC=(u32)address;
    *D_800ABBD0=0;
    *(u32 *)D_800ABBD4=0x01000401;
}
