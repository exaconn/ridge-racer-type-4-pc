/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/spu.h"

s32 SsUtSetVVol(s32 input, s32 left, s32 right) {
    register s32 voice = input;
    if ((u16)voice < 24) {
        register s32 result;
        /* ASM_BARRIER: Keep the narrowed right volume in the original arithmetic register. */
        register s32 right_value __asm__("$3");
        register s32 right_scaled;
        register s32 index;
        register s32 offset;
        register s32 left_value;
        register s32 left_scaled;
        /* ASM_BARRIER: Preserve the PAL page-address register used for the cache flag load. */
        register u8 *base __asm__("$5");
        /* ASM_BARRIER: Reuse the flag-address register after its last pointer use. */
        register u32 flags __asm__("$5");
        /* ASM_BARRIER: Materialize the success value before narrowing and scaling the right volume. */
        __asm__("" : "=r"(result) : "0"(0) : "$3");
        right_value = (s16)right;
        /* ASM_BARRIER: Keep the signed halfword conversion before multiplication. */
        __asm__("" : "=r"(right_value) : "0"(right_value));
        right_scaled = right_value * 129;

        index = (s16)voice;
        offset = index * 16;
        ((SpuCachedVolumeEntry *)((u8 *)_svm_sreg_dirty.voices + offset))->right = right_scaled;
        left_value = (s16)left;

        left_scaled = left_value * 128;
        /* ASM_BARRIER: Preserve cache-store and flag-load scheduling and register lifetimes. */
        __asm__ volatile("" : : "r"(index), "r"(left_scaled) : "$5", "memory");
        base = (u8 *)0x80110000U;
        /* ASM_BARRIER: Expose the PAL page base; base[-0x1E8 + voice] aliases _svm_sreg_dirty.flags. */
        __asm__("" : "=r"(base) : "0"(base));
        base += index;
        flags = base[-0x1E8];
        left_scaled += left_value;
        ((SpuCachedVolumeEntry *)((u8 *)_svm_sreg_dirty.voices + offset))->left = left_scaled;
        _svm_sreg_dirty.flags[index] = flags | 3;
        return result;
    }
    return -1;
}
