/* CC1_FLAGS: -mno-split-addresses */
#include "rr/cd.h"
s32 func_8008DCB4(u8 *a, u32 b) {
    volatile int x;
    *D_800A674C = 0;
    *D_800A6758 = 0x80;
    *D_800A6780 = 0x21020843;
    *D_800A675C = 0x1325;
    *D_800A6784 |= 0x8000;
    *D_800A6788 = a;
    *D_800A678C = b | 0x10000;
    while (!(*D_800A674C & 0x40)) {
    }
    *D_800A6790 = 0x11400100;
    x = *D_800A6790;
    return 0;
}
