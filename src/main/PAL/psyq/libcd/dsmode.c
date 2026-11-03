#include "rr/library.h"
#include "psyq/libcd/s_1.h"

extern s32 func_8008C140(s32 command, const u8 *parameters, u8 *result);
extern void func_8008C55C(RrLibCompletionCallback);
extern void func_8008F5E4(void), func_8008F248(void);

void func_8008F1C4(s32 flags) {
    u8 buf[8];
    buf[0] = flags;
    func_8008C140(0xE, buf, 0);
    if (flags & 0x100) {
        if (flags & 0x20) {
            {
                /* ASM_BARRIER: Retain the assembler temporary for the mode-flag store. */
                register s32 *base __asm__("$1");
                /* ASM_BARRIER: Materialize the mode-store address only after its conditional branch. */
                __asm__ volatile("" : : : "$1", "memory");
                /* PAL mapping: base[0x348 / 4] is D_80110348. */
                base = (s32 *)0x80110000U;
                /* ASM_BARRIER: Expose the PAL mode store without a symbol-address macro. */
                __asm__("" : "=r"(base) : "0"(base));
                base[0x348 / 4] = 0;
            }
        } else {
            {
                s32 *base;
                register s32 one = 1;
                /* PAL mapping: base[0x348 / 4] is D_80110348. */
                base = (s32 *)0x80110000U;
                base[0x348 / 4] = one;
            }
        }
        func_8008C55C(func_8008F5E4);
        func_8008C12C(func_8008F248);
    }
    {
        const u8 *zero = 0;
        /* ASM_BARRIER: Keep the two null arguments independently initialized as in the original call. */
        __asm__("" : "=r"(zero) : "0"(zero));
        func_8008C140(0x1B, zero, 0);
    }
}
