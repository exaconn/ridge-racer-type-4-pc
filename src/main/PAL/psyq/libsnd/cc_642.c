/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"
#include "psyq/libsnd/vmtick2.h"
void _SsContDamper(s32 raw_bank, s32 raw_row, s32 raw_value) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    LibStateB0 *state = &_ss_score[bank][row];
    u32 value = raw_value & 255;
    if (value < 64) _SsVmDamperOff();
    else _SsVmDamperOn();
    state->w90 = _SsReadDeltaValue(bank, row);
}
