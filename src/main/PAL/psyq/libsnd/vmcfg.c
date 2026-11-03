/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libspu/sminit.h"
#include "rr/spu_allocation.h"


void func_8008A674(u16 channel);

s32 func_8008A644(u32 count) {
    s32 result;
    if (((count - 1) & 0xFFU) < 24) {
        D_801100FC = count;
        result = (s8)count;
    } else {
        result = -1;
    }
    return result;
}

void func_8008A674(u16 channel) {
    s32 index;
    if (channel < 16) {
        /* ASM_BARRIER: Preserve the original indexed-load register. */
        register u8 *flags __asm__("$3");
        index = (s16)channel;
        /* ASM_BARRIER: Normalize the bank index before materializing its flag page. */
        __asm__ volatile("" : : "r"(index) : "$3");
        /* PAL mapping: flags[0x120 + index] is D_80110120[index]. */
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
            /* PAL mapping: allocations + offset + 0x180 is &D_80110180[index]. */
            allocations = (u8 *)0x80110000U;
            /* ASM_BARRIER: Expose the PAL allocation-page base for the argument load. */
            __asm__("" : "=r"(allocations) : "0"(allocations));
            allocations += offset;
            func_8008A6F4(*(u32 *)(allocations + 0x180));
            D_80110120[index] = 0;
            D_80110178--;
        }
    }
}

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
