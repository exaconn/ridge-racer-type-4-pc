#include "rr/channels.h"
#include "psyq/libsnd/vmchval.h"

/* The low byte selects a bank; the next byte selects its sequence row. */
s32 _SsVmGetSeqVol(s32 input, u16 *left, u16 *right) {
    SequenceChannel *record = _ss_score[input & 0xFF];
    s16 *last = &_svm_cur_vab_id;
    *last = input;
    record += (input & 0xFF00) >> 8;
    *left = record->h58[0];
    *right = record->h58[1];
    /* An output may alias the selected-ID cell or the other volume field. */
    return *last;
}

s32 _SsVmGetSeqLVol(s32 input) {
    SequenceChannel *record = _ss_score[input & 0xFF];
    _svm_cur_vab_id = input;
    record += (input & 0xFF00) >> 8;
    return (s16)record->h58[0];
}

s32 _SsVmGetSeqRVol(s32 input) {
    SequenceChannel *record = _ss_score[input & 0xFF];
    _svm_cur_vab_id = input;
    record += (input & 0xFF00) >> 8;
    return (s16)record->h58[1];
}
