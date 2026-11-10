/* CC1_FLAGS: -mno-split-addresses */

#include "rr/library.h"

extern void _SsGetSeqData(s32, s32);

void _SsSeqPlay(s32 bank, s32 row) {
    u32 bank_bits = (u32)bank << 16;
    /* ASM_BARRIER: Retain the original register across record lookup and counter updates. */
    register u32 row_bits __asm__("$5");
    /* ASM_BARRIER: Retain the original register across record lookup and counter updates. */
    register s32 index __asm__("$4");
    /* ASM_BARRIER: Retain the original register across record lookup and counter updates. */
    register LibStateB0 **table __asm__("$3");
    LibStateB0 *state;
    s32 units;
    s32 period;
    s16 delay;
    s32 current, delta;
    u32 accumulated;
    s32 result;
    register u32 reset_value;
    index = (s16)(bank_bits >> 16) * (s32)sizeof(*table);
    /* ASM_BARRIER: Complete the bank index before forming the preserved row bits. */
    __asm__("" : "=r"(row) : "0"(row));
    row_bits = (u32)row << 16;
    units = (s16)(row_bits >> 16) * 11;
    /* ASM_BARRIER: Complete both index calculations before materializing the PAL lookup page. */
    __asm__ volatile("" : : "r"(index), "r"(units) : "$3");
    /* PAL mapping: table[-0xF98 / 4] selects _ss_score[bank]. */
    table = (LibStateB0 **)0x80110000U;
    /* ASM_BARRIER: Expose the PAL lookup page without folding its signed low offset. */
    __asm__("" : "=r"(table) : "0"(table));
    table = (LibStateB0 **)((u8 *)table + index);
    state = (LibStateB0 *)((u8 *)table[-0xF98 / 4] + units * 16);
    /* h48[5] is the delay at 0x52; h48[6] is the period at 0x54. */
    period = state->h48[6];
    current = state->w90;
    /* ASM_BARRIER: Load the current counter before the unsigned period snapshot. */
    __asm__ volatile("" : : "r"(current) : "memory");
    reset_value = (u16)state->h48[6];
    /* ASM_BARRIER: Keep the unsigned reload value available before the fast-path branches. */
    __asm__("" : "=r"(reset_value) : "0"(reset_value));
    delta = (s32)((u32)current - (u32)period);
    if (delta > 0) {
        delay = state->h48[5];
        if (delay > 0) {
            state->h48[5] = (u16)state->h48[5] - 1;
            return;
        }
        if (delay == 0) {
            state->h48[5] = reset_value;
            result = (s32)((u32)state->w90 - 1);
            goto finish;
        }
        state->w90 = delta;
        return;
    }
    if (period >= current) {
        register s32 saved_bank;
        register s32 saved_row;
        register s32 update;
        register s32 next_period;
        accumulated = (u32)current;
        saved_bank = (s32)bank_bits;
        saved_row = (s32)row_bits;
        do {
again:
            _SsGetSeqData(saved_bank >> 16, saved_row >> 16);
            update = state->w90;
            if (update == 0) goto again;
            next_period = state->h48[6];
            accumulated += (u32)update;
        } while ((s32)accumulated < next_period);
        result = (s32)(accumulated - (u32)next_period);
finish:
        state->w90 = result;
    }
}
