#include "rr/library.h"
void _SsContRpn1(s32 raw_bank, s32 raw_row, s32 raw_value) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    s8 value = raw_value;
    /* ASM_BARRIER: Retain the original table/record register across the update call. */
    register LibStateB0 **table __asm__("$16");
    register s32 index = bank * sizeof(*table);
    register s32 units;
    LibStateB0 *state;
    units = row * 11;
    /* ASM_BARRIER: Complete index arithmetic before materializing the PAL table page. */
    __asm__ volatile("" : : "r"(index), "r"(units) : "$16");
    /* PAL mapping: table[-0xF98 / 4] selects _ss_score[bank]. */
    table = (LibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL table page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (LibStateB0 **)((u8 *)table + index);
    state = (LibStateB0 *)((u8 *)table[-0xF98 / 4] + units * 16);
    state->b18 = value;
    state->b1e++;
    state->w90 = _SsReadDeltaValue(bank, row);
}
