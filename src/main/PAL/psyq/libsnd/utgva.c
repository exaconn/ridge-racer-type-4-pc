/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libsnd/libsnd.h"
#include "rr/channels.h"
#include "rr/spu_tone_snapshot.h"

s32 SsUtGetVagAtr(s32 a0, s16 a1, s32 a2, u8 *a3) {
    s16 bank = a0;
    s16 idx;
    VagAtrBuffer *selected;
    if (_svm_vab_used[bank] == 1) {
        _SsVmVSetUp(bank, a1);
        idx = (u32)a2 + (u32)(_svm_cur.f07 * 16);
        a3[VAB_TONE_PRIORITY] = _svm_tn[idx][VAB_TONE_PRIORITY];
        a3[VAB_TONE_MODE] = _svm_tn[idx][VAB_TONE_MODE];
        a3[VAB_TONE_VOLUME] = _svm_tn[idx][VAB_TONE_VOLUME];
        a3[VAB_TONE_PAN] = _svm_tn[idx][VAB_TONE_PAN];
        a3[VAB_TONE_CENTER] = _svm_tn[idx][VAB_TONE_CENTER];
        a3[VAB_TONE_SHIFT] = _svm_tn[idx][VAB_TONE_SHIFT];
        a3[VAB_TONE_MAX_NOTE] = _svm_tn[idx][VAB_TONE_MAX_NOTE];
        a3[VAB_TONE_MIN_NOTE] = _svm_tn[idx][VAB_TONE_MIN_NOTE];
        a3[VAB_TONE_VIBRATO_DEPTH] = _svm_tn[idx][VAB_TONE_VIBRATO_DEPTH];
        a3[VAB_TONE_VIBRATO_TIME] = _svm_tn[idx][VAB_TONE_VIBRATO_TIME];
        a3[VAB_TONE_PORTAMENTO_DEPTH] = _svm_tn[idx][VAB_TONE_PORTAMENTO_DEPTH];
        a3[VAB_TONE_PORTAMENTO_TIME] = _svm_tn[idx][VAB_TONE_PORTAMENTO_TIME];
        a3[VAB_TONE_BEND_MIN] = _svm_tn[idx][VAB_TONE_BEND_MIN];
        a3[VAB_TONE_BEND_MAX] = _svm_tn[idx][VAB_TONE_BEND_MAX];
        selected = _svm_tn;
        *(u16 *)&a3[VAB_TONE_ADSR1] = *(u16 *)&selected[idx][VAB_TONE_ADSR1];
        *(u16 *)&a3[VAB_TONE_ADSR2] = *(u16 *)&selected[idx][VAB_TONE_ADSR2];
        *(u16 *)&a3[VAB_TONE_PROGRAM] = *(u16 *)&selected[idx][VAB_TONE_PROGRAM];
        *(u16 *)&a3[VAB_TONE_SAMPLE] = *(u16 *)&selected[idx][VAB_TONE_SAMPLE];
        return 0;
    }
    return -1;
}
