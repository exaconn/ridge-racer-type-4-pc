/* GCC_VERSION: 2.7.2 */
#include "rr/spu.h"
extern s32 D_800A48B8;
extern RrSpuAllocation *D_800A48C0;
extern void func_8008A774(void);

void func_8008A6F4(u32 address) {
    s32 visited = 0;
    s32 count = D_800A48B8;
    if (count > 0) {
        u32 end_marker = 0x40000000U;
        u32 freed = address | 0x80000000U;
        s32 limit = count;
        RrSpuAllocation *entry;

        /* ASM_BARRIER: materialize loop invariants before loading the cursor. */
        __asm__ volatile("" : : "r"(end_marker), "r"(freed), "r"(limit));
        entry = D_800A48C0;
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
    func_8008A774();
}
