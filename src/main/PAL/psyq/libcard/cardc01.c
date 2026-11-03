/* GCC_VERSION: 2.7.2 */
#include "rr/commands.h"

extern s32 func_8009C3BC(void), func_8009C400(s32);

s32 func_8009C310(s32 a0, u32 *a1);
int func_8009C3BC();

s32 func_8009C310(s32 a0, u32 *a1) {
    int r;
    if (!(D_800ABDE4 & 1)) {
        r = 0;
    } else {
        D_800ABDF4 = 0; D_800ABDF8 = 0; D_80114730 = 0; D_800ABDE8 = 0; D_800ABDEC = a0;
        r = 1;
    }
    if (!r) return 0;
    D_80114724 = -2;
    D_80114870 = 1;
    D_80114734 = func_8009C3BC;
    D_80114738 = func_8009C400;
    D_8011471C = a1;
    D_800ABDE4 = 2;
    return 1;
}

int func_8009C3BC() {
    int a; 
    register int r __asm__("$2");
    int t; 
    register int c; 
    a = D_800ABDE8;
    t = 2;
    if (a == t) {
        r = 0x59;
        goto out;
    }
    r = 0;
    if (a < 2) goto out;
    if (a >= 6) goto out;
    c = a < 4;
    if (c) goto out;
    /* ASM_BARRIER: stops 2.7.2 if-converting the last compare into xori/negu/andi */
    __asm__("" : "=r"(r) : "0"(0xFF));
out:
    return r;
}
