/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"
void _SsContMainVol(s32 raw_bank, s32 raw_row, s32 raw_value) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    s8 value = raw_value;
    LibStateB0 *state = &_ss_score[bank][row];
    u8 slot = state->b17;
    u8 *selected = (u8 *)state + slot;
    _SsVmSetVol((s16)(raw_bank | (raw_row * 0x100U)), state->b26, selected[RR_FIELD_OFFSET(LibStateB0, b37)], (u8)value, selected[RR_FIELD_OFFSET(LibStateB0, b27)]);
    state->h60[slot] = (u8)value;
    state->w90 = _SsReadDeltaValue(bank, row);
}
