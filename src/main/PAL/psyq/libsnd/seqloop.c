/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"
void _SsContRpn1(s32 raw_bank, s32 raw_row, s32 raw_value) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    s8 value = raw_value;
    LibStateB0 *state = &_ss_score[bank][row];
    state->b18 = value;
    state->b1e++;
    state->w90 = _SsReadDeltaValue(bank, row);
}
