#include "rr/channel_stream.h"
void _SsSndTempo(s32 bankInput, s32 rowInput) {
    s16 bank = bankInput;
    s16 row = rowInput;
    ChannelStreamB0 *record = &_ss_score[bank][row];
    s32 remaining = (u32)record->wa8 - 1U;
    s32 interval;
    u32 value, pitch;
    u32 nextValue;
    s32 crossed;
    record->wa8 = remaining;
    if (remaining < 0) {
        _ss_score[bank][row].w98 &= ~0x40;
        _ss_score[bank][row].w98 &= ~0x80;
        return;
    }
    interval = record->h48[3];
    if (interval > 0) {
        u32 target;
        if (remaining % interval != 0) return;
        value = record->w94;
        target = record->wac;
        if (target < value) nextValue = value - 1;
        else if ((crossed = value < target)) nextValue = value + 1;
        else goto adjusted;
        record->w94 = nextValue;
    } else {
        u32 target;
        value = record->w94;
        target = record->wac;
        if (target < value) {
            nextValue = value + interval;
            record->w94 = nextValue;
            crossed = nextValue < target;
        } else {
            if (!(crossed = value < target)) goto adjusted;
            nextValue = value - interval;
            record->w94 = nextValue;
            target = record->wac;
            crossed = target < nextValue;
        }
        if (crossed) record->w94 = target;
    }
adjusted:
    pitch = record->h48[4] * record->w94;
    pitch = pitch * 10U / (VBLANK_MINUS * 60U);
    record->h48[6] = (s16)pitch;
    if ((s16)pitch <= 0) record->h48[6] = 1;
    if (record->wa8 == 0 || record->w94 == record->wac) {
        _ss_score[bank][row].w98 &= ~0x40;
        _ss_score[bank][row].w98 &= ~0x80;
    }
}
