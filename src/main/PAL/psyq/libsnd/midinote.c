/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"
#include "psyq/libsnd/vmmain.h"
extern void func_80088340(s32, s32, s32, s32);
void func_80081CB4(s32 bank, s32 row, s32 value, s32 mode) {
    /* ASM_BARRIER: Retain the original register across the bank lookup and callback dispatch. */
    register s32 raw_bank __asm__("$8") = bank;
    register s32 raw_value = value;
    /* ASM_BARRIER: Retain the original register across the bank lookup and callback dispatch. */
    register RrLibStateB0 **table __asm__("$3");
    s32 index;
    s32 units, offset;
    register RrLibStateB0 *state;
    register s32 mode_byte;
    register s32 previous;
    RrLibStateB0 *bank_base;
    u8 *selected;
    s32 slot;
    /* ASM_BARRIER: Retain the original register across the bank lookup and callback dispatch. */
    register u32 row_bits __asm__("$4");
    /* ASM_BARRIER: Preserve the raw bank before normalizing its lookup index. */
    __asm__("" : "=r"(raw_bank) : "0"(raw_bank) : "$4");
    /* ASM_BARRIER: Preserve the value before its incoming register becomes the selected-slot pointer. */
    __asm__("" : "=r"(raw_value) : "0"(raw_value) : "$6");
    index = (s16)raw_bank * (s32)sizeof(*table);
    units = (s16)row * 11;
    offset = units * 16;
    /* ASM_BARRIER: Complete both offsets before materializing the PAL table page. */
    __asm__ volatile("" : : "r"(index), "r"(offset) : "$3");
    /* PAL mapping: table[-0xF98 / 4] selects D_8010F068[bank]. */
    table = (RrLibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL lookup page without a symbol-address macro. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (RrLibStateB0 **)((u8 *)table + index);
    bank_base = table[-0xF98 / 4];
    /* ASM_BARRIER: Load the bank base before normalizing the callback mode. */
    __asm__("" : "=r"(mode) : "0"(mode), "r"(bank_base));
    mode_byte = mode & 255;
    /* ASM_BARRIER: Retain the complete mode byte for the fifth callback argument. */
    __asm__("" : "=r"(mode_byte) : "0"(mode_byte));
    state = (RrLibStateB0 *)((u8 *)bank_base + offset);
    /* The active slot selects one of the sixteen b27/b37 entries. */
    slot = state->b17;
    selected = (u8 *)state + slot;
    previous = selected[0x27];
    /* ASM_BARRIER: Snapshot the selected-slot metadata before choosing the callback path. */
    __asm__("" : "=r"(previous) : "0"(previous));
    if (mode & 255) {
        if(!((state->h80 >> slot) & 1)) {
            row_bits = (u32)row << 8;
            func_80087DE4((s16)(raw_bank | row_bits), state->b26, selected[0x37], raw_value & 255, mode_byte, previous);
        }
    } else {
        row_bits = (u32)row << 8;
        func_80088340((s16)(raw_bank | row_bits), state->b26, selected[0x37], raw_value & 255);
    }
}
