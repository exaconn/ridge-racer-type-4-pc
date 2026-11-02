/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
void func_800805D4(s32 bank, s32 row, s32 value) {
    /* ASM_BARRIER: Retain the original saved or argument register across record lookup and callbacks. */
    register u32 raw_bank __asm__("$18") = bank;
    /* ASM_BARRIER: Retain the original saved or argument register across record lookup and callbacks. */
    register u32 raw_row __asm__("$20") = row;
    register s32 bank_value;
    register s32 row_value;
    /* ASM_BARRIER: Retain the original saved or argument register across record lookup and callbacks. */
    register RrLibStateB0 **table __asm__("$16");
    register u32 slot;
    register u8 *selected;
    register u16 *pitch;
    RrLibStateB0 *state;
    register s32 device;
    s32 index, units;
    bank_value = (s16)raw_bank;
    index = bank_value * (s32)sizeof(*table);
    row_value = (s16)raw_row;
    units = row_value * 11;
    /* ASM_BARRIER: Complete both indices before materializing the PAL lookup page. */
    __asm__ volatile("" : : "r"(index), "r"(units) : "$16");
    /* PAL mapping: table[-0xF98 / 4] selects D_8010F068[bank]. */
    table = (RrLibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL lookup page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (RrLibStateB0 **)((u8 *)table + index);
    state = table[-0xF98 / 4] + row_value;
    /* The active slot indexes the sixteen per-slot entries. */
    slot = state->b17;
    device = state->b26;
    
    selected = (u8 *)state + slot;
    func_80089624(device, selected[RR_FIELD_OFFSET(RrLibStateB0, b37)], value & 255);
    raw_row <<= 8;
    raw_bank |= raw_row;
    raw_bank <<= 16;
    pitch = (u16 *)(slot * sizeof(u16) + (unsigned long)state);
    func_80089FA4((s32)raw_bank >> 16, state->b26,
                 selected[RR_FIELD_OFFSET(RrLibStateB0, b37)],
                 *(u16 *)((u8 *)pitch + RR_FIELD_OFFSET(RrLibStateB0, h60)),
                 selected[RR_FIELD_OFFSET(RrLibStateB0, b27)]);
    state->w90 = func_80081E04(bank_value, row_value);
}
