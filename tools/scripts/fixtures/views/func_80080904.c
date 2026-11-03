/* CC1_FLAGS: -mno-split-addresses */

#include "rr/library.h"

void func_80080904(s32 bank, s32 row, s32 payload) {
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register u32 raw_bank __asm__("$8") = bank;
    register u32 raw_row = row;
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register u32 value __asm__("$7") = payload;
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register RrLibStateB0 **table __asm__("$1");
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register s32 index __asm__("$3");
    RrLibStateB0 *base;
    RrLibStateB0 *state;
    s32 normalized_bank, normalized_row, units;
    u32 count, masked_count;
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register u32 bank_bits __asm__("$4");
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register u32 row_bits __asm__("$5");
    /* ASM_BARRIER: Preserve raw selectors and value before reusing incoming argument registers. */
    __asm__("" : "+r"(bank), "+r"(row) : "r"(raw_bank), "r"(raw_row), "r"(value));
    normalized_bank = (s16)bank;
    index = normalized_bank * sizeof(*table);
    /* ASM_BARRIER: Scale the bank before forming the row stride. */
    __asm__("" : "+r"(index) : : "$2");
    normalized_row = (s16)row;
    units = normalized_row * 11;
    /* ASM_BARRIER: Finish selector arithmetic before materializing the PAL page. */
    __asm__ volatile("" : : "r"(index), "r"(units) : "$1");
    table = (RrLibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL lookup page without folding its signed low offset. */
    __asm__("" : "+r"(table));
    table = (RrLibStateB0 **)((u8 *)table + index);
    base = table[-0xF98 / 4];
    state = base + normalized_row;
    switch (value & 255) {
    case 20:
        state->b1b = payload;
        state->b1c = 1;
        state->w90 = func_80081E04(normalized_bank, normalized_row);
        state->c8 = state->c0;
        break;
    case 30:
        count = state->b1d;
        masked_count = count & 255;
        state->b1b = payload;
        if (masked_count == 0) {
            state->b15 = 0;
            goto decode;
        }
        if (masked_count < 127) {
            state->b1d = count - 1;
            state->w90 = func_80081E04(normalized_bank, normalized_row);
            if (state->b1d) state->c0 = state->c8;
            else state->b15 = 0;
        } else {
            func_80081E04(normalized_bank, normalized_row);
            state->w90 = 0;
            state->c0 = state->c8;
        }
        break;
    default:
        bank_bits = raw_bank << 16;
        normalized_bank = (s32)bank_bits >> 16;
        row_bits = raw_row << 16;
        normalized_row = (s32)row_bits >> 16;
        state->b1b = value;
        state->b1f++;
    decode:
        state->w90 = func_80081E04(normalized_bank, normalized_row);
        break;
    }
}
