/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"

extern s32 D_800A6498;
extern void func_8008DDAC(void);

void CD_initintr(void) {
    D_800A648C = 0;
    D_800A6488 = 0;
    D_800A6498 = 0;
    {
        /* ASM_BARRIER: retain the original assembler temporary for the final state store. */
        register s32 *base __asm__("$1");
        /* ASM_BARRIER: complete preceding clears before materializing the last state address. */
        __asm__ volatile("" : : : "$1", "memory");
        /* PAL mapping: base[0x6494 / 4] is D_800A6494. */
        base = (s32 *)0x800A0000U;
        /* ASM_BARRIER: expose the last clear in the driver-reset call's delay slot. */
        __asm__("" : "=r"(base) : "0"(base));
        base[0x6494 / 4] = 0;
    }
    func_8008B364();
    func_8008B394(2, func_8008DDAC);
}
