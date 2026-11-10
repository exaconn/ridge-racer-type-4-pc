/* CC1_FLAGS: -mno-split-addresses */

#include "rr/library.h"

extern s32 _SsVmPitchBend(s32, s32, s32, s32);

extern LibStateB0 *_ss_score[];

void _SsSetPitchBend(s32 bank, s32 row) {
    s32 bank_value = bank;
    /* ASM_BARRIER: Retain the original saved or argument register through stream lookup and callbacks. */
    register s32 row_value __asm__("$17") = row;
    /* ASM_BARRIER: Retain the original saved or argument register through stream lookup and callbacks. */
    register u32 encoded_bits __asm__("$4");
    /* ASM_BARRIER: Retain the original saved or argument register through stream lookup and callbacks. */
    register LibStateB0 **table __asm__("$16");
    s32 index, units;
    LibStateB0 *state;
    u8 *cursor, *selected;
    u32 value; u8 slot;
    encoded_bits = (u32)row_value<< 8;
    encoded_bits = ((u32)bank_value | encoded_bits)<< 16;
    bank_value = (s16)bank_value;
    index = bank_value * (s32)sizeof(*table);
    /* ASM_BARRIER: Finish bank scaling before normalizing the preserved row. */
    __asm__("" : "=r"(row_value) : "0"(row_value), "r"(index));
    row_value = (s16)row_value;
    units = row_value * 11;
    /* ASM_BARRIER: Complete both lookup indices before materializing the PAL table page. */
    __asm__ volatile("" : : "r"(index), "r"(units) : "$16");
    /* PAL mapping: table[-0xF98 / 4] selects _ss_score[bank]. */
    table = (LibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL lookup page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (LibStateB0 **)((u8 *)table + index);
    state = table[-0xF98 / 4] + row_value;
    cursor = state->c0;
    value = *cursor++;
    state->c0 = cursor;
    slot = state->b17;
    selected = slot + state->b37;
    _SsVmPitchBend((s32)encoded_bits>> 16, state->b26, selected[0], value);
    state->w90 = _SsReadDeltaValue(bank_value, row_value);
}
