/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/spu.h"

void SsVabClose(u16 channel) {
    s32 index;
    if (channel < 16) {
        /* ASM_BARRIER: Preserve the original indexed-load register. */
        register u8 *flags __asm__("$3");
        index = (s16)channel;
        /* ASM_BARRIER: Normalize the bank index before materializing its flag page. */
        __asm__ volatile("" : : "r"(index) : "$3");
        /* PAL mapping: flags[0x120 + index] is _svm_vab_used[index]. */
        flags = (u8 *)0x80110000U;
        /* ASM_BARRIER: Expose the PAL flag-page base without folding the symbol offset. */
        __asm__("" : "=r"(flags) : "0"(flags));
        flags += index;
        if (flags[0x120] == 1) {
            /* ASM_BARRIER: Preserve the original indexed-load register. */
            register u8 *allocations __asm__("$4");
            s32 offset = index * sizeof(u32);
            /* ASM_BARRIER: Scale the allocation index before materializing its address page. */
            __asm__ volatile("" : : "r"(offset) : "$4");
            /* PAL mapping: allocations + offset + 0x180 is &_svm_vab_start[index]. */
            allocations = (u8 *)0x80110000U;
            /* ASM_BARRIER: Expose the PAL allocation-page base for the argument load. */
            __asm__("" : "=r"(allocations) : "0"(allocations));
            allocations += offset;
            SpuFree(*(u32 *)(allocations + 0x180));
            _svm_vab_used[index] = 0;
            _svm_vab_count--;
        }
    }
}
