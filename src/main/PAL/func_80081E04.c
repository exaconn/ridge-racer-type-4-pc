/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"
extern RrLibStateB0 *D_8010F068[];
s32 func_80081E04(s32 raw_bank, s32 raw_row) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    /* ASM_BARRIER: Preserve the original decoder register allocation and overlapping lifetimes. */
    register RrLibStateB0 **table __asm__("$3");
    register RrLibStateB0 *bank_base;
    register RrLibStateB0 *state;
    /* ASM_BARRIER: Preserve the original decoder register allocation and overlapping lifetimes. */
    register u8 *cursor __asm__("$2");
    /* ASM_BARRIER: Preserve the original decoder register allocation and overlapping lifetimes. */
    register u32 result __asm__("$2");
    u32 value, next;
    table = D_8010F068;
    table += bank;
    bank_base = *table;
    state = bank_base + row;
    cursor = state->c0;
    value = *cursor;
    cursor++;
    state->c0 = cursor;
    result = 0;
    if (value != 0) {
        if (value & 128) {
            value &= 127;
            do {
                cursor = state->c0;
                next = *cursor;
                cursor++;
                state->c0 = cursor;
                value = (value << 7) + (next & 127);
            } while (next & 128);
        }
        result = value * 4;
        result = (result + value) * 2;
        state->w88 = (u32)state->w88 + result;
    }
    return result;
}
