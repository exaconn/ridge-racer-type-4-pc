#include "rr/library.h"

void _SsContNrpn1(s32 bank, s32 row, s32 payload);
void _SsContNrpn2(s32 bank, s32 row, s32 payload);

void _SsContNrpn1(s32 bank, s32 row, s32 payload) {
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register u32 raw_bank __asm__("$18") = bank;
    register u32 raw_row = row;
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register s32 value __asm__("$17");
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register LibStateB0 **table __asm__("$1");
    register s32 normalized_bank;
    register s32 normalized_row;
    LibStateB0 *base;
    LibStateB0 *state;
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register s32 index __asm__("$3");
    s32 units;
    LibControllerCallback callback;
    LibControllerCallback *callbacks;
    register LibControllerCallback *callback_base;
    /* ASM_BARRIER: Keep incoming selectors distinct from their saved copies. */
    __asm__("" : "+r"(bank), "+r"(row) : "r"(raw_bank), "r"(raw_row) : "$17");
    normalized_bank = (s16)bank;
    index = normalized_bank * sizeof(*table);
    /* ASM_BARRIER: Scale the bank before forming the row stride. */
    __asm__("" : "+r"(index) : : "$2");
    normalized_row = (s16)row;
    units = normalized_row * 11;
    /* ASM_BARRIER: Finish selector arithmetic before materializing the PAL page. */
    __asm__ volatile("" : : "r"(index), "r"(units) : "$1");
    table = (LibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL table page without folding its signed low offset. */
    __asm__("" : "+r"(table));
    table = (LibStateB0 **)((u8 *)table + index);
    base = table[-0xF98 / 4];
    state = base + normalized_row;
    value = payload;
    if (state->b1b == 40) {
        callback_base = (LibControllerCallback *)_SsMarkCallback;
        callbacks = (LibControllerCallback *)(normalized_bank * 16 * sizeof(*callbacks) + (unsigned long)callback_base);
        callback = *(normalized_row + callbacks);
        if (callback) callback(normalized_bank, normalized_row, value & 255);
    }
    if (state->b1b != 30 && state->b1b != 20 && state->b1b != 40) {
        state->b1a = value;
        state->b1c = 0;
        state->b1f++;
    }
    state->w90 = _SsReadDeltaValue((s16)raw_bank, (s16)raw_row);
}

void _SsContNrpn2(s32 bank, s32 row, s32 payload) {
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register u32 raw_bank __asm__("$8") = bank;
    register u32 raw_row = row;
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register u32 value __asm__("$7") = payload;
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register LibStateB0 **table __asm__("$1");
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register s32 index __asm__("$3");
    LibStateB0 *base;
    LibStateB0 *state;
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
    table = (LibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL lookup page without folding its signed low offset. */
    __asm__("" : "+r"(table));
    table = (LibStateB0 **)((u8 *)table + index);
    base = table[-0xF98 / 4];
    state = base + normalized_row;
    switch (value & 255) {
    case 20:
        state->b1b = payload;
        state->b1c = 1;
        state->w90 = _SsReadDeltaValue(normalized_bank, normalized_row);
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
            state->w90 = _SsReadDeltaValue(normalized_bank, normalized_row);
            if (state->b1d) state->c0 = state->c8;
            else state->b15 = 0;
        } else {
            _SsReadDeltaValue(normalized_bank, normalized_row);
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
        state->w90 = _SsReadDeltaValue(normalized_bank, normalized_row);
        break;
    }
}
