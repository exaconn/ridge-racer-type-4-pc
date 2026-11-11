#include "rr/channels.h"
#include "rr/library.h"
#include "psyq/libsnd/vmchval.h"
#include "psyq/libsnd/ssvolset.h"
#include "psyq/libsnd/ssatr2.h"

void _SsSndSetVol(s32 bank, s32 row, s32 left, s32 right);
void SsSeqSetVol(s16 bank, s32 left, s32 right);
void SsSepSetVol(s32 bank, s32 row, s32 left, s32 right);
void SsSeqGetVol(s32 bank, s32 row, u16 *left, u16 *right);

void _SsSndSetVol(s32 raw_bank, s32 raw_row, s32 raw_left, s32 raw_right) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    s16 left = raw_left;
    s16 right = raw_right;
    SequenceChannel *state = &_ss_score[bank][row];
    if (state->w98 != 1) {
        state->h58[0] = left;
        state->h58[1] = right;
    } else {
        _SsVmSetSeqVol((s16)(raw_bank | (raw_row * 0x100U)), (u16)left, (u16)right, 1);
    }
}

void SsSeqSetVol(s16 bank, s32 raw_left, s32 raw_right) {
    s16 left = raw_left;
    s16 right = raw_right;
    SequenceChannel *state = _ss_score[bank];
    if (state->w98 != 1) {
        state->h58[0] = left;
        state->h58[1] = right;
    } else {
        _SsVmSetSeqVol(bank, (u16)left, (u16)right, 1);
    }
}

void SsSepSetVol(s32 raw_bank, s32 raw_row, s32 raw_left, s32 raw_right) {
    s16 bank = raw_bank;
    s16 row = raw_row;
    s16 left = raw_left;
    s16 right = raw_right;
    SequenceChannel *state = &_ss_score[bank][row];
    if (state->w98 != 1) {
        state->h58[0] = left;
        state->h58[1] = right;
    } else {
        _SsVmSetSeqVol((s16)(raw_bank | (raw_row * 0x100U)), (u16)left, (u16)right, 1);
    }
}

void SsSeqGetVol(s32 bank, s32 row, u16 *left, u16 *right) {
    _SsVmGetSeqVol((s16)(bank | (row * 0x100U)), left, right);
}
