/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/spu.h"

s32 SsUtSetDetVVol(s32 voice, s32 left, s32 right) {
    s32 i;
    if ((u16)voice < 24) {
        /* ASM_BARRIER: Preserve the PAL page-address register used for the cache flag load. */
        register u8 *base __asm__("$4");
        register u32 flags;
        i = (s16)voice;
        _svm_sreg_dirty.voices[i].right = right;
        /* ASM_BARRIER: Preserve cache-store and flag-load scheduling and register lifetimes. */
        __asm__ volatile("" : : "r"(i) : "$4", "memory");
        base = (u8 *)0x80110000U;
        /* ASM_BARRIER: Expose the PAL page base; base[-0x1E8 + voice] aliases _svm_sreg_dirty.flags. */
        __asm__("" : "=r"(base) : "0"(base));
        base += i;
        flags = base[-0x1E8];
        _svm_sreg_dirty.voices[i].left = left;
        _svm_sreg_dirty.flags[i] = flags | 3;
        return 0;
    }
    return -1;
}
