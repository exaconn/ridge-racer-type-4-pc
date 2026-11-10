/* CC1_FLAGS: -mno-split-addresses */

#include "rr/library.h"

void _SsContExternal(s32 raw_bank, s32 raw_row, s32 raw_value) {
    u32 bank_selector = raw_bank;
    register u32 value = raw_value & 255;
    register u32 bank_bits;
    register u32 row_bits;
    register u32 left;
    register u32 right;
    /* ASM_BARRIER: Preserve the original register while preparing the paired channel arguments. */
    register LibStateB0 **table __asm__("$16");
    LibStateB0 *state;
    s32 bank, row, index, units;
    bank_bits = bank_selector << 16;
    left = value;
    /* ASM_BARRIER: Preserve the bank before preparing the first argument. */
    __asm__("" : "+r"(left) : "r"(bank_bits) : "$17");
    row_bits = (u32)raw_row << 16;
    right = left;
    /* ASM_BARRIER: Preserve the row before preparing the second argument and normalizing the bank. */
    __asm__("" : "+r"(right) : "r"(row_bits));
    bank = (s32)bank_bits >> 16;
    index = bank * sizeof(*table);
    row = (s32)row_bits >> 16;
    units = row * 11;
    /* ASM_BARRIER: Finish the selectors before materializing the PAL page. */
    __asm__ volatile("" : : "r"(units), "r"(index));
    table = (LibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL lookup page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (LibStateB0 **)((u8 *)table + index);
    state = table[-0xF98 / 4] + row;
    SsUtSetReverbDepth(left, right);
    state->w90 = _SsReadDeltaValue(bank, row);
}
