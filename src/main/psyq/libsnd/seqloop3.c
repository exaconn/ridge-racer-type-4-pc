#include "rr/library.h"
#include "psyq/libsnd/vmtick.h"
void _SsContResetAll(s32 raw_bank, s32 raw_row) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    LibStateB0 *state = &_ss_score[bank][row];
    SsUtReverbOff();
    _SsVmDamperOff();
    { s32 slot = state->b17; state->b37[slot] = slot; }
    state->b18 = 0;
    state->b19 = 0;
    *(state->b17 + state->h60) = 0x7F;
    state->b27[state->b17] = 0x40;
    state->w90 = _SsReadDeltaValue(bank, row);
}
