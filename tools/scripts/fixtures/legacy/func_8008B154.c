/* CC1_FLAGS: -mno-split-addresses -fno-schedule-insns */
extern volatile unsigned *D_800A5258;
extern volatile unsigned *D_800A525C;
extern volatile unsigned D_800A5260, D_800A5264;
#include "rr/timing.h"
int func_8008B154(a0) int a0; {
    volatile unsigned t;
    unsigned s0;
    unsigned s1;
    volatile unsigned *r;
    register int v __asm__("$2"); /* ASM_BARRIER: first call argument stays in v0 like the original */
    int w;
    s0 = *D_800A5258;
    do {
        t = *D_800A525C;
    } while (t != *D_800A525C);
    s1 = (t - D_800A5260) & 0xFFFF;
    if (a0 < 0) return D_800A6390;
    if (a0 == 1) return s1;
    if (a0 > 0) {
        v = D_800A5264;
        v = v - 1;
        v = v + a0;
    } else {
        v = D_800A5264;
    }
    w = 0;
    if (a0 > 0) w = a0 - 1;
    func_8008B2CC(v, w);
    s0 = *D_800A5258;
    func_8008B2CC(D_800A6390 + 1, 1);
    if (s0 & 0x400000) {
        r = D_800A5258;
        if ((int)(s0 ^ *r) >= 0) {
            do {
            } while (((s0 ^ *r) & 0x80000000) == 0);
        }
    }
    D_800A5264 = D_800A6390;
    do {
        D_800A5260 = *D_800A525C;
    } while (D_800A5260 != *D_800A525C);
    return s1;
}
