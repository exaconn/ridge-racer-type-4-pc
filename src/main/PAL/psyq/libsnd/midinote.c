/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"
#include "rr/channels.h"
#include "psyq/libsnd/vmmain.h"
#include "psyq/libsnd/midinote.h"
void _SsNoteOn(s32 bank, s32 row, s32 value, s32 mode) {
    s16 selected_bank = bank;
    s16 selected_row = row;
    LibStateB0 *base = _ss_score[selected_bank];
    u32 mode_byte = (u8)mode;
    LibStateB0 *state = base + selected_row;
    u32 slot = state->b17;
    s32 previous = state->b27[slot];
    if (mode & 255) {
        if (!((state->h80 >> slot) & 1)) {
            _SsVmKeyOn((s16)(bank | (row * 0x100U)), state->b26,
                        state->b37[slot], value & 255, mode_byte, previous);
        }
    } else {
        _SsVmKeyOff((s16)(bank | (row * 0x100U)), state->b26,
                     state->b37[slot], value & 255);
    }
}
