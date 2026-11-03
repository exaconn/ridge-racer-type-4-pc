/* GCC_VERSION: 2.7.2 */
#include "rr/counters.h"
int func_80096FA4(unsigned a0, unsigned short a1, int a2) {
    int t = a0 & 0xFFFF;
    int a3 = 0x48;
    char *p;
    if (t >= 3) return 0;
    p = (char *)(t * 16 + (unsigned long)D_800ABC34);
    *(short *)(p + 4) = 0;
    *(volatile short *)(p + 8) = a1;
    if ((unsigned)t < 2) {
        if (a2 & 0x10) a3 = 0x49;
        if (!(a2 & 1)) a3 |= 0x100;
    } else {
        if (t == 2) {
            if (!(a2 & 1)) a3 = 0x248;
        }
    }
    if (a2 & 0x1000) a3 |= 0x10;
    *(short *)(t * 16 + (unsigned long)D_800ABC34 + 4) = a3;
    return 1;
}
