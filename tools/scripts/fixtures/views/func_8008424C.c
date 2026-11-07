/* GCC_VERSION: 2.7.2 */

#include "rr/library.h"

extern void _SsVmSetSeqVol(s32,s32,s32,s32);

void SsSepSetVol(s32 bank, s32 row, s32 left, s32 right) {
    /* ASM_BARRIER: Preserve the original register allocation across lookup and the mode branch. */
    register s32 raw_bank __asm__("$8") = bank;
    /* ASM_BARRIER: Preserve the original register allocation across lookup and the mode branch. */
    register s32 left_value __asm__("$9") = left;
    /* ASM_BARRIER: Preserve the original register allocation across lookup and the mode branch. */
    register s32 right_value __asm__("$10");
    /* ASM_BARRIER: Preserve the original register allocation across lookup and the mode branch. */
    register RrLibStateB0 **table __asm__("$3");
    register RrLibStateB0 *bank_base;
    s32 index;
    s32 units;
    RrLibStateB0 *state;
    /* ASM_BARRIER: Preserve the original register allocation across lookup and the mode branch. */
    register u32 row_bits __asm__("$4");
    /* ASM_BARRIER: Retain the left value before reusing its incoming argument register for lookup. */
    __asm__("" : "=r"(left_value) : "0"(left_value) : "$4");
    index = (s16)bank * sizeof(*table);
    units = (s16)row * 11;
    /* ASM_BARRIER: Complete index arithmetic before materializing the PAL table page. */
    __asm__ volatile("" : : "r"(index), "r"(units) : "$3");
    /* PAL mapping: table[-0xF98 / 4] selects _ss_score[bank]. */
    table = (RrLibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL table page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (RrLibStateB0 **)((u8 *)table + index);
    bank_base = table[-0xF98 / 4];
    /* ASM_BARRIER: Load the bank base before forming the final record address. */
    __asm__("" : "=r"(bank_base) : "0"(bank_base));
    state = bank_base + (s16)row;
    right_value = right;
    if (state->w98 != 1) {
        state->h58[0] = left;
        state->h58[1] = right;
        return;
    }
    row_bits = (u32)row << 8;
    _SsVmSetSeqVol((s16)(raw_bank | row_bits), left_value & 0xFFFF, right_value & 0xFFFF, 1);
}
