/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channel_stream.h"
#include "psyq/libsnd/sstick.h"
#include "psyq/libsnd/ssdelay.h"

void _SsSeqPlay(s32 bankInput, s32 rowInput) {
    s16 bank = bankInput;
    s16 row = rowInput;
    ChannelStreamB0 *state = &_ss_score[bank][row];

    if (state->accumulator90 - state->pitch54 > 0) {
        if (state->delay52 > 0) {
            state->delay52--;
            return;
        }
        if (state->delay52 == 0) {
            state->delay52 = state->pitch54;
            state->accumulator90--;
        } else {
            state->accumulator90 -= state->pitch54;
        }
        return;
    }
    if (state->pitch54 >= state->accumulator90) {
        s32 accumulated = state->accumulator90;
        do {
            do {
                _SsGetSeqData(bank, row);
            } while (state->accumulator90 == 0);
            accumulated += state->accumulator90;
        } while (accumulated < state->pitch54);
        state->accumulator90 = accumulated - state->pitch54;
    }
}

void _SsSeqGetEof(s32 bankInput, s32 rowInput) {
    s16 bank = bankInput;
    s16 row = rowInput;
    ChannelStreamB0 *record = &_ss_score[bank][row];
    s8 elapsed = record->elapsed21 + 1;

    record->elapsed21 = elapsed;
    if (record->duration20 == 0) {
        record->accumulator88 = 0;
        record->state1C = 0;
        record->accumulator90 = 0;
        if (_ss_score[bank][row].mode98 & 0x400) {
            record->cursor00 = record->alternate0C;
        } else {
            record->cursor00 = record->start04;
        }
    } else if (elapsed < record->duration20) {
        record->accumulator88 = 0;
        record->state1C = 0;
        record->accumulator90 = 0;
        if (_ss_score[bank][row].mode98 & 0x400) {
            record->cursor00 = record->alternate0C;
            record->saved08 = record->alternate0C;
        } else {
            record->cursor00 = record->start04;
            record->saved08 = record->start04;
        }
    } else {
        _ss_score[bank][row].mode98 &= ~1;
        _ss_score[bank][row].mode98 &= ~8;
        _ss_score[bank][row].mode98 &= ~2;
        _ss_score[bank][row].mode98 |= 0x200;
        _ss_score[bank][row].mode98 |= 4;
        record->active14 = 0;
        if (_ss_score[bank][row].mode98 & 0x400) {
            record->saved08 = record->alternate0C;
        } else {
            record->saved08 = record->start04;
        }
        if (record->restartBank22 != -1) {
            record->active14 = 0;
            _SsSndNextSep(record->restartBank22, record->restartRow23);
            _SsVmSeqKeyOff((s16)(bank | (row << 8)));
        }
        _SsVmSeqKeyOff((s16)(bank | (row << 8)));
        record->accumulator90 = record->pitch54;
    }
}
