#include "rr/library.h"
void _SsContPanpot(s32 raw_bank, s32 raw_row, s32 raw_value) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    s8 value = raw_value;
    s32 encoded = (s16)(bank | ((u32)row << 8));
    LibStateB0 *state = &_ss_score[bank][row];
    u8 slot = state->b17;
    _SsVmSetVol(encoded, state->b26, state->b37[slot], (u16)state->h60[slot], value & 255);
    state->b27[slot] = value;
    state->w90 = _SsReadDeltaValue(bank, row);
}
