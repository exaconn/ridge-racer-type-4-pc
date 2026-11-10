/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libsnd/libsnd.h"
#include "rr/channels.h"
#include "rr/spu_tone_snapshot.h"

s32 SsUtGetVagAtr(s32 a0, s16 a1, s32 a2, u8 *a3) {
    s16 bank = a0;
    s16 idx;
    VagAtr *dst = (VagAtr *)a3;
    if (_svm_vab_used[bank] == 1) {
        _SsVmVSetUp(bank, a1);
        idx = a2 + _svm_cur.f07 * 16;
        dst->f00 = ((VagAtr *)_svm_tn)[idx].f00;
        dst->f01 = ((VagAtr *)_svm_tn)[idx].f01;
        dst->f02 = ((VagAtr *)_svm_tn)[idx].f02;
        dst->f03 = ((VagAtr *)_svm_tn)[idx].f03;
        dst->f04 = ((VagAtr *)_svm_tn)[idx].f04;
        dst->f05 = ((VagAtr *)_svm_tn)[idx].f05;
        dst->f07 = ((VagAtr *)_svm_tn)[idx].f07;
        dst->f06 = ((VagAtr *)_svm_tn)[idx].f06;
        dst->f08 = ((VagAtr *)_svm_tn)[idx].f08;
        dst->f09 = ((VagAtr *)_svm_tn)[idx].f09;
        dst->f0A = ((VagAtr *)_svm_tn)[idx].f0A;
        dst->f0B = ((VagAtr *)_svm_tn)[idx].f0B;
        dst->f0C = ((VagAtr *)_svm_tn)[idx].f0C;
        dst->f0D = ((VagAtr *)_svm_tn)[idx].f0D;
        dst->f10 = ((VagAtr *)_svm_tn)[idx].f10;
        dst->f12 = ((VagAtr *)_svm_tn)[idx].f12;
        dst->f14 = ((VagAtr *)_svm_tn)[idx].f14;
        dst->f16 = ((VagAtr *)_svm_tn)[idx].f16;
        return 0;
    }
    return -1;
}
