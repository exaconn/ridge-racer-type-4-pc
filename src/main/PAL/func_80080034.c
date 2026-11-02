/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
void func_80080034(s16 bank, s16 row) {
    /* ASM_BARRIER: Retain the original table/record register across the update call. */
    register RrLibStateB0 **table __asm__("$16");
    register s32 index = bank * (s32)sizeof(*table);
    register s32 units;
    RrLibStateB0 *state;
    units = row * 11;
    /* ASM_BARRIER: Complete index arithmetic before materializing the PAL table page. */
    __asm__ volatile("" : : "r"(index), "r"(units) : "$16");
    /* PAL mapping: table[-0xF98 / 4] selects D_8010F068[bank]. */
    table = (RrLibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL table page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (RrLibStateB0 **)((u8 *)table + index);
    state = table[-0xF98 / 4] + row;
    {
        u8 *cursor = state->c0;
        u8 value = *cursor++;
        state->c0 = cursor;
        state->b26 = value;
    }
    state->w90 = func_80081E04(bank, row);
}
