#include "rr/library.h"
void _SsContExternal(s32 raw_bank, s32 raw_row, s32 raw_value) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    u32 value = raw_value & 255;
    LibStateB0 *state = &_ss_score[bank][row];
    SsUtSetReverbDepth(value, value);
    state->w90 = _SsReadDeltaValue(bank, row);
}
