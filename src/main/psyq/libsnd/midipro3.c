#include "rr/sequencer_init.h"
#include "psyq/libsnd/ssatr.h"

void _SsSetPitchBend(s32 bankInput, s32 rowInput) {
    s16 bank = bankInput;
    s16 row = rowInput;
    LibStateB0 *state = &_ss_score[bank][row];
    u8 value = *state->c0++;

    _SsVmPitchBend((s16)(bank | (row << 8)), state->b26, *(state->b17 + state->b37), value);
    state->w90 = _SsReadDeltaValue(bank, row);
}

void _SsSetControlChange(s32 bankInput, s32 rowInput, s32 controlInput) {
    s16 bank = bankInput;
    s16 row = rowInput;
    u8 control = controlInput;
    LibStateB0 *state = &_ss_score[bank][row];
    u8 value = *state->c0++;

    switch (control) {
    case 0:
        state->b26 = value;
        state->w90 = _SsReadDeltaValue(bank, row);
        break;
    case 6:
        SsCtrlHandlerCC6(bank, row, value);
        break;
    case 7:
        D_8010EFE4(bank, row, value);
        break;
    case 10:
        D_8010EFE8(bank, row, value);
        break;
    case 11:
        D_8010EFEC(bank, row, value);
        break;
    case 64:
        SsCtrlHandlerCC64(bank, row, value);
        break;
    case 91:
        D_8010F004(bank, row, value);
        break;
    case 98:
        SsCtrlHandlerCC98(bank, row, value);
        break;
    case 99:
        D_8010EFF8(bank, row, value);
        break;
    case 100:
        D_8010EFFC(bank, row, value);
        break;
    case 101:
        D_8010F000(bank, row, value);
        break;
    case 121:
        SsCtrlHandlerCC121(bank, row);
        break;
    default:
        state->w90 = _SsReadDeltaValue(bank, row);
        break;
    }
}
