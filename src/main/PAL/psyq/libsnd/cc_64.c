/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"
void _SsContExpression(s32 raw_bank, s32 raw_row, s32 raw_value) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    LibStateB0 *state = &_ss_score[bank][row];
    u32 slot = state->b17;
    s32 device = state->b26;
    _SsVmSetProgVol(device, state->b37[slot], raw_value & 255);
    _SsVmSetVol((s16)(raw_bank | (raw_row * 0x100U)), state->b26,
                state->b37[slot], (u16)state->h60[slot], state->b27[slot]);
    state->w90 = _SsReadDeltaValue(bank, row);
}
