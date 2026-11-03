/* CC1_FLAGS: -mno-split-addresses */
#include "rr/commands.h"

extern s32 func_8009DF14(s32, s32);
extern s32 func_8009CA8C(void);
extern s32 func_8009CB50(s32);

s32 func_8009C978(s32 request, u8 *buffer, s32 sector, s32 input_count) {
    s32 result;
    s32 active;
    /* ASM_BARRIER: retain the count in the original callee-saved register across validation calls. */
    register s32 count __asm__("$17") = input_count;
    /* ASM_BARRIER: finish preserving count before preparing the first validation call. */
    __asm__("" : "=r"(count) : "0"(count));
    if (!func_8009DF14(1, sector)) return 0;
    if (!func_8009DF14(1, (s32)((u32)sector + (u32)count - 1))) return 0;
    if (!rr_command_begin(request)) return 0;
    /* ASM_BARRIER: keep success initialization before the command configuration stores. */
    __asm__ volatile("" : "=r"(result) : "0"(1) : "$3", "memory");
    D_80114870 = 5;
    D_80114734 = func_8009CA8C;
    D_80114738 = func_8009CB50;
    active = 2;
    D_80114718 = buffer;
    D_80114724 = sector;
    D_80114728 = count;
    {
        /* ASM_BARRIER: preserve the original assembler temporary for the mapped store. */
        register s32 *base __asm__("$1");
        /* ASM_BARRIER: finish parameter publication before materializing the state address. */
        __asm__ volatile("" : : : "$1", "memory");
        /* PAL RAM mapping: base[-0x421C / 4] is D_800ABDE4. */
        base = (s32 *)0x800B0000U;
        /* ASM_BARRIER: expose the active-state store in the branch delay slot. */
        __asm__("" : "=r"(base) : "0"(base));
        base[-0x421C / 4] = active;
    }
    return result;
}
