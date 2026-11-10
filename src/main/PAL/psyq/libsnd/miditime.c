/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 970318 */
#include "rr/library.h"
#include "rr/sequencer_init.h"
s32 _SsReadDeltaValue(s32 raw_bank, s32 raw_row) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    LibStateB0 *state = &_ss_score[bank][row];
    u8 *cursor = state->c0;
    u32 value = *cursor++;
    u32 next;
    u32 result;
    state->c0 = cursor;
    if (value == 0) {
        return 0;
    }
    if (value & 128) {
        value &= 127;
        do {
            u8 *cursor2 = state->c0;
            next = *cursor2++;
            state->c0 = cursor2;
            value = (value << 7) + (next & 127);
        } while (next & 128);
    }
    result = value * 10;
    state->w88 = state->w88 + result;
    return result;
}
