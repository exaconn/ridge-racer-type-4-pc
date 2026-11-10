#include "rr/library.h"
#include "psyq/libsnd/vmtick.h"
void _SsContResetAll(s32 raw_bank, s32 raw_row) {
    s16 bank = raw_bank;
    s16 row = raw_row;
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
    SsUtReverbOff();
    _SsVmDamperOff();
    { s32 slot = state->b17; ((u8 *)state + slot)[0x37] = slot; }
    state->b18 = 0;
    state->b19 = 0;
    *(state->b17 + state->h60) = 0x7F;
    state->b27[state->b17] = 0x40;
    state->w90 = _SsReadDeltaValue(bank, row);
}
