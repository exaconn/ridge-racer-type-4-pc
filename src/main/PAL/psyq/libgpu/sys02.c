/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "m2c_macros.h"

extern unsigned char D_800ABAC1[];
extern int D_80011E50[];
extern int (*D_800ABABC)();
extern int *D_800ABAB8;
extern int func_8008B3C4();
extern u8 D_800ABAC2;

int func_80092BA4();
u8 func_80092C48(void);

int func_80092BA4(a0) int a0; {
    register unsigned char *p __asm__("$17"); /* ASM_BARRIER: original keeps the pointer in s1 and a0 in s0 */
    int old;
    p = D_800ABAC1;
    old = p[0];
    if (p[1] >= 2) {
        D_800ABABC(D_80011E50, a0);
    }
    if (a0 != p[0]) {
        ((int (*)())D_800ABAB8[0x34 / 4])(1);
        p[0] = a0;
        func_8008B3C4(2, 0);
    }
    return old;
}

u8 func_80092C48(void) {
    return D_800ABAC2;
}
