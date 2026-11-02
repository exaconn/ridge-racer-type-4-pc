/* CC1_FLAGS: -mno-split-addresses */
#include "rr/draw_environment.h"
extern int func_80096444();
extern char D_80011F4C[];
extern int *D_800ABAB8;
extern int (*D_800ABABC)();
extern unsigned char D_800ABAC2;
int func_800933B4(int *a0) {
    unsigned char *p = &D_800ABAC2;
    if (*p >= 2) {
        D_800ABABC(D_80011F4C, a0);
    }
    func_80093E70((u32 *)(a0 + 7), (RrFrameEnvironment *)a0);
    a0[7] |= 0xFFFFFF;
    ((int (*)())D_800ABAB8[2])(D_800ABAB8[6], a0 + 7, 0x40, 0);
    func_80096444(p + 0xE, a0, 0x5C);
    return (int)a0;
}
