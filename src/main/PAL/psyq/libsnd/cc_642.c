/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
extern void func_80087044(void), func_80087054(void);
void func_800806C4(s32 bank, s32 row, s32 raw_value) {
    /* ASM_BARRIER: Preserve the original register through selector calculation and call argument preparation. */
    register u32 raw_bank __asm__("$17") = bank;
    u32 value = raw_value;
    /* ASM_BARRIER: Preserve the original register through selector calculation and call argument preparation. */
    register u32 raw_row __asm__("$18") = row;
    register RrLibStateB0 **table;
    RrLibStateB0 *state;
    /* ASM_BARRIER: Preserve the original register through selector calculation and call argument preparation. */
    register s32 index __asm__("$4");
    register s32 selected_row;
    /* ASM_BARRIER: Preserve the original register through selector calculation and call argument preparation. */
    register s32 units __asm__("$2");
    /* ASM_BARRIER: Preserve the original register through selector calculation and call argument preparation. */
    register u32 bank_bits __asm__("$4");
    register u32 row_bits;
    /* ASM_BARRIER: Keep incoming selector registers distinct from their saved copies. */
    __asm__("" : "+r"(bank), "+r"(row) : "r"(raw_bank), "r"(raw_row));
    index = (s16)bank * sizeof(*table);
    selected_row = (s16)row;
    units = selected_row * sizeof(*state);
    value &= 255;
    /* ASM_BARRIER: Finish the selectors and value before materializing the PAL page. */
    __asm__ volatile("" : "+r"(value) : "r"(units), "r"(index));
    table = (RrLibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL lookup page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (RrLibStateB0 **)((u8 *)table + index);
    state = (RrLibStateB0 *)((u8 *)table[-0xF98 / 4] + units);
    if ((u32)value < 64) func_80087044();
    else func_80087054();
    bank_bits = raw_bank << 16;
    row_bits = raw_row << 16;
    /* ASM_BARRIER: Shift the row before completing signed bank normalization for the decoder. */
    __asm__("" : "+r"(row_bits) : "r"(bank_bits));
    state->w90 = func_80081E04((s32)bank_bits >> 16, (s32)row_bits >> 16);
}
