/* CC1_FLAGS: -mno-split-addresses */

#include "rr/library.h"

void func_80080434(s32 raw_bank, s32 raw_row, s32 raw_value) {
    s32 bank = (s16)raw_bank;
    register s32 row;
    /* ASM_BARRIER: Retain the original register for selector, slot, or callback argument scheduling. */
    register s32 value __asm__("$16");
    /* ASM_BARRIER: Retain the original register for selector, slot, or callback argument scheduling. */
    register RrLibStateB0 **table __asm__("$17");
    register RrLibStateB0 *state;
    register u32 slot;
    u8 *selected;
    u16 *pitch;
    s32 index, units, encoded;
    register u32 pitch_arg;
    /* ASM_BARRIER: Retain the original register for selector, slot, or callback argument scheduling. */
    register u32 channel __asm__("$3");
    index = bank * sizeof(*table);
    row = (s16)raw_row;
    units = row * sizeof(*state);
    /* ASM_BARRIER: Keep raw selector encoding after the complete table indices. */
    __asm__("" : "+r"(raw_row) : "r"(units), "r"(index));
    encoded = (s16)((u32)raw_bank | ((u32)raw_row << 8));
    /* ASM_BARRIER: Encode the channel before preserving the raw pitch input. */
    __asm__("" : "=r"(encoded) : "0"(encoded), "r"(units), "r"(index) : "$16");
    value = raw_value;
    /* ASM_BARRIER: Finish the selectors and preserve the unmasked input before materializing the lookup page. */
    __asm__ volatile("" : "+r"(value) : "r"(units), "r"(encoded), "r"(index) : "$17");
    table = (RrLibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL lookup page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (RrLibStateB0 **)((u8 *)table + index);
    state = (RrLibStateB0 *)((u8 *)table[-0xF98 / 4] + units);
    pitch_arg = value & 255;
    slot = state->b17;
    selected = (u8 *)state + slot;
    channel = selected[RR_FIELD_OFFSET(RrLibStateB0, b37)];
    func_80089FA4(encoded, state->b26,
                 channel, pitch_arg,
                 selected[RR_FIELD_OFFSET(RrLibStateB0, b27)]);
    pitch = (u16 *)(slot * sizeof(u16) + (unsigned long)state);
    *(u16 *)((u8 *)pitch + RR_FIELD_OFFSET(RrLibStateB0, h60)) = value & 255;
    state->w90 = func_80081E04(bank, row);
}
