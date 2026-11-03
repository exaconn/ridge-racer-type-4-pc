/* GCC_VERSION: 2.7.2 */

#include "rr/commands.h"

extern s32 func_8009C3BC(void), func_8009C400(s32);

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
