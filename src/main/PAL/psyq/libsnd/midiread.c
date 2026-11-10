/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"
#include "psyq/libsnd/midiread.h"
void _SsContBankChange(s16 bank, s16 row) {
    LibStateB0 *state = &_ss_score[bank][row];
    {
        u8 *cursor = state->c0;
        u8 value = *cursor++;
        state->c0 = cursor;
        state->b26 = value;
    }
    state->w90 = _SsReadDeltaValue(bank, row);
}
