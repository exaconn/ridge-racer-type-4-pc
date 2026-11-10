/* CC1_FLAGS: -mno-split-addresses */

#include "rr/library.h"

void _SsContPanpot(s32 raw_bank, s32 raw_row, s32 raw_value) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    s8 value = raw_value;
    /* ASM_BARRIER: Keep the selected record in the original saved register across both calls. */
    register LibStateB0 **table __asm__("$16");
    register s32 encoded = (s16)(bank | ((u32)row << 8));
    s32 index, units;
    LibStateB0 *state;
    u8 slot;
    u8 *selected;
    index = bank * sizeof(*table);
    units = row * 11;
    /* ASM_BARRIER: Finish index arithmetic before materializing the PAL table page. */
    __asm__ volatile("" : : "r"(index), "r"(units) : "$16");
    /* PAL mapping: table[-0xF98 / 4] selects _ss_score[bank]. */
    table = (LibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL table page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (LibStateB0 **)((u8 *)table + index);
    state = (LibStateB0 *)((u8 *)table[-0xF98 / 4] + units * 16);
    slot = state->b17;
    selected = (u8 *)state + slot;
    _SsVmSetVol(encoded, state->b26, selected[0x37], *(slot + state->h60), value & 255);
    selected[0x27] = value;
    state->w90 = _SsReadDeltaValue(bank, row);
}
