/* CC1_FLAGS: -mno-split-addresses */

#include "rr/library.h"

void _SsContNrpn1(s32 bank, s32 row, s32 payload) {
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register u32 raw_bank __asm__("$18") = bank;
    register u32 raw_row = row;
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register s32 value __asm__("$17");
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register RrLibStateB0 **table __asm__("$1");
    register s32 normalized_bank;
    register s32 normalized_row;
    RrLibStateB0 *base;
    RrLibStateB0 *state;
    /* ASM_BARRIER: Retain the original register for selector snapshots, lookup, or call scheduling. */
    register s32 index __asm__("$3");
    s32 units;
    RrLibControllerCallback callback;
    RrLibControllerCallback *callbacks;
    register RrLibControllerCallback *callback_base;
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
    table = (RrLibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL table page without folding its signed low offset. */
    __asm__("" : "+r"(table));
    table = (RrLibStateB0 **)((u8 *)table + index);
    base = table[-0xF98 / 4];
    state = base + normalized_row;
    value = payload;
    if (state->b1b == 40) {
        callback_base = (RrLibControllerCallback *)_SsMarkCallback;
        callbacks = (RrLibControllerCallback *)(normalized_bank * 16 * sizeof(*callbacks) + (unsigned long)callback_base);
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
