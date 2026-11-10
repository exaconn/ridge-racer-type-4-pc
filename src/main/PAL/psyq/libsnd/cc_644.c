/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"

void _SsContNrpn1(s32 raw_bank, s32 raw_row, s32 raw_value) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    u8 value = raw_value;
    LibStateB0 *state = &_ss_score[bank][row];
    if (state->b1b == 40) {
        LibControllerCallback *callback_base = (LibControllerCallback *)_SsMarkCallback;
        LibControllerCallback *callbacks = (LibControllerCallback *)(bank * 16 * sizeof(*callbacks) + (unsigned long)callback_base);
        LibControllerCallback callback = *(row + callbacks);
        if (callback) callback(bank, row, value);
    }
    if (state->b1b != 30 && state->b1b != 20 && state->b1b != 40) {
        state->b1a = value;
        state->b1c = 0;
        state->b1f++;
    }
    state->w90 = _SsReadDeltaValue(bank, row);
}

void _SsContNrpn2(s32 raw_bank, s32 raw_row, s32 raw_value) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    u8 value = raw_value;
    LibStateB0 *state = &_ss_score[bank][row];
    switch (value) {
    case 20:
        state->b1b = value;
        state->b1c = 1;
        state->w90 = _SsReadDeltaValue(bank, row);
        state->c8 = state->c0;
        break;
    case 30:
        state->b1b = value;
        if (state->b1d == 0) {
            state->b15 = 0;
            state->w90 = _SsReadDeltaValue(bank, row);
        } else if (state->b1d < 127) {
            state->b1d--;
            state->w90 = _SsReadDeltaValue(bank, row);
            if (state->b1d) state->c0 = state->c8;
            else state->b15 = 0;
        } else {
            _SsReadDeltaValue(bank, row);
            state->w90 = 0;
            state->c0 = state->c8;
        }
        break;
    default:
        state->b1b = value;
        state->b1f++;
        state->w90 = _SsReadDeltaValue(bank, row);
        break;
    }
}
