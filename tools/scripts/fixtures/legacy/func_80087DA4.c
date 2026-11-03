/* GCC_VERSION: 2.7.2 */
#include "types.h"
extern s32 D_800A480C;
void func_80087DA4(s32 mode) {
    if (mode == 1) {
        /* ASM_BARRIER: preserve the original assembler temporary for the mapped store. */
        register s32 *base __asm__("$1");
        /* ASM_BARRIER: finish the preceding state updates before binding the address. */
        __asm__ volatile("" : : : "$1");
        /* PAL RAM mapping for D_800A480C. */
        base = (s32 *)0x800A0000U;
        /* ASM_BARRIER: allow the register-relative store in the branch delay slot. */
        __asm__("" : "=r"(base) : "0"(base));
        base[0x480C / 4] = 0;
    } else {
        D_800A480C = 1;
    }
}
