#include "rr/channel_stream.h"
#include "psyq/libsnd/sstick.h"
#include "psyq/libsnd/ssdelay.h"

void _SsSeqPlay(s32 bankInput, s32 rowInput) {
    s16 bank = bankInput;
    s16 row = rowInput;
    ChannelStreamB0 *state = &_ss_score[bank][row];

    if ((s32)((u32)state->w90 - state->h48[6]) > 0) {
        if (state->h48[5] > 0) {
            state->h48[5]--;
            return;
        }
        if (state->h48[5] == 0) {
            state->h48[5] = state->h48[6];
            state->w90 = (u32)state->w90 - 1U;
        } else {
            state->w90 = (u32)state->w90 - state->h48[6];
        }
        return;
    }
    if (state->h48[6] >= state->w90) {
        s32 accumulated = state->w90;
        do {
            do {
                _SsGetSeqData(bank, row);
            } while (state->w90 == 0);
            accumulated = (u32)accumulated + state->w90;
        } while (accumulated < state->h48[6]);
        state->w90 = (u32)accumulated - state->h48[6];
    }
}

void _SsSeqGetEof(s32 bankInput, s32 rowInput) {
    s16 bank = bankInput;
    s16 row = rowInput;
    ChannelStreamB0 *record = &_ss_score[bank][row];
    s8 elapsed = record->b21 + 1;

    record->b21 = elapsed;
    if (record->b20 == 0) {
        record->w88 = 0;
        record->b1c = 0;
        record->w90 = 0;
        if (_ss_score[bank][row].w98 & 0x400) {
            record->c0 = record->cC;
        } else {
            record->c0 = record->c4;
        }
    } else if (elapsed < record->b20) {
        record->w88 = 0;
        record->b1c = 0;
        record->w90 = 0;
        if (_ss_score[bank][row].w98 & 0x400) {
            record->c0 = record->cC;
            record->c8 = record->cC;
        } else {
            record->c0 = record->c4;
            record->c8 = record->c4;
        }
    } else {
        _ss_score[bank][row].w98 &= ~1;
        _ss_score[bank][row].w98 &= ~8;
        _ss_score[bank][row].w98 &= ~2;
        _ss_score[bank][row].w98 |= 0x200;
        _ss_score[bank][row].w98 |= 4;
        record->b14 = 0;
        if (_ss_score[bank][row].w98 & 0x400) {
            record->c8 = record->cC;
        } else {
            record->c8 = record->c4;
        }
        if (record->b22[0] != -1) {
            record->b14 = 0;
            _SsSndNextSep(record->b22[0], record->b22[1]);
            _SsVmSeqKeyOff((s16)(bank | ((u32)row << 8)));
        }
        _SsVmSeqKeyOff((s16)(bank | ((u32)row << 8)));
        record->w90 = record->h48[6];
    }
}
