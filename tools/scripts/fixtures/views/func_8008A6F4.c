/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/spu.h"

extern s32 D_800A48B8;

extern void _spu_gcSPU(void);

void SpuFree(u32 address) {
    s32 visited = 0;
    s32 count = D_800A48B8;
    if (count > 0) {
        u32 end_marker = 0x40000000U;
        u32 freed = address | 0x80000000U;
        s32 limit = count;
        RrSpuAllocation *entry;

        /* ASM_BARRIER: materialize loop invariants before loading the cursor. */
        __asm__ volatile("" : : "r"(end_marker), "r"(freed), "r"(limit));
        entry = _spu_memList;
        /* ASM_BARRIER: place the pointer's load delay before the loop label. */
        __asm__ volatile("nop");
        do {
            /* ASM_BARRIER: reuse v1 for the current entry's address and flags. */
            register u32 flags __asm__("$3") = entry->address_flags;
            if (flags & end_marker) break;
            visited++;
            if (flags == address) {
                entry->address_flags = freed;
                break;
            }
            entry++;
        } while (visited < limit);
    }
    _spu_gcSPU();
}
