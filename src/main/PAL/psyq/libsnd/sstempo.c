/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channel_stream.h"
void _SsSndTempo(s32 bankInput, s32 rowInput) {
    s16 bank = bankInput;
    s16 row = rowInput;
    ChannelStreamB0 *record = &_ss_score[bank][row];
    s32 remaining = record->tempoRemainingA8 - 1;
    s32 interval;
    u32 value, pitch;
    u32 nextValue;
    s32 crossed;
    record->tempoRemainingA8 = remaining;
    if (remaining < 0) {
        _ss_score[bank][row].mode98 &= ~0x40;
        _ss_score[bank][row].mode98 &= ~0x80;
        return;
    }
    interval = record->interval4E;
    if (interval > 0) {
        u32 target;
        if (remaining % interval != 0) return;
        value = record->tempo94;
        target = record->targetTempoAC;
        if (target < value) nextValue = value - 1;
        else if ((crossed = value < target)) nextValue = value + 1;
        else goto adjusted;
        record->tempo94 = nextValue;
    } else {
        u32 target;
        value = record->tempo94;
        target = record->targetTempoAC;
        if (target < value) {
            nextValue = value + interval;
            record->tempo94 = nextValue;
            crossed = nextValue < target;
        } else {
            if (!(crossed = value < target)) goto adjusted;
            nextValue = value - interval;
            record->tempo94 = nextValue;
            target = record->targetTempoAC;
            crossed = target < nextValue;
        }
        if (crossed) record->tempo94 = target;
    }
adjusted:
    pitch = record->scale50 * record->tempo94;
    pitch = pitch * 10U / (VBLANK_MINUS * 60U);
    record->pitch54 = (s16)pitch;
    if ((s16)pitch <= 0) record->pitch54 = 1;
    if (record->tempoRemainingA8 == 0 || record->tempo94 == record->targetTempoAC) {
        _ss_score[bank][row].mode98 &= ~0x40;
        _ss_score[bank][row].mode98 &= ~0x80;
    }
}
