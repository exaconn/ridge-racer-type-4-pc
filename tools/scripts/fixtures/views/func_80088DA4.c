/* GCC_VERSION: 2.7.2 */

#include "rr/channels.h"

extern u16 D_80110000;

extern u16 D_80110002;

extern u16 D_8010F8F8;

extern u16 D_8010F8FA;

extern u8 D_8010F925[];

extern u8 D_8010F90C[];

/* The selected channel belongs to the valid 0..31 control-row range. */
void _SsVmKeyOffNow(s32 unused_mode) {
    s32 selected = D_80110118;
    u32 index = selected & 0xFFFF;
    u32 high_mask, low_mask;
    s32 offset;
    if (index < 16) {
        low_mask = 1U << index;
        high_mask = 0;
    } else {
        low_mask = 0;
        index -= 16;
        high_mask = 1U << index;
        index = selected & 0xFFFF;
    }
    offset = index * sizeof(RrChannelControlRow36);
    D_8010F925[offset] = 0;
    {
        u32 low = D_80110000;
        u32 high = D_80110002;
        *(u16 *)(D_8010F90C + offset) = 0;
        _svm_voice[index].value00 = 0;
        {
            u32 pending = D_8010F8F8;
            low |= low_mask;
            D_80110000 = low;
            D_8010F8F8 = pending & ~low;
        }
        {
            u32 pending = D_8010F8FA;
            high |= high_mask;
            D_80110002 = high;
            {
                /* ASM_BARRIER: retain the original assembler temporary for the final mask store. */
                register u16 *base __asm__("$1");
                u32 result = pending & ~high;
                /* ASM_BARRIER: finish mask arithmetic and state updates before materializing its address. */
                __asm__ volatile("" : : "r"(result) : "$1", "memory");
                /* PAL mapping: base[-0x706 / 2] is D_8010F8FA. */
                base = (u16 *)0x80110000U;
                /* ASM_BARRIER: expose the final halfword store in the return delay slot. */
                __asm__("" : "=r"(base) : "0"(base));
                base[-0x706 / 2] = result;
            }
        }
    }
}
