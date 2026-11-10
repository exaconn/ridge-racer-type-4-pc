/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "psyq/libsnd/libsnd.h"
#include "rr/channels.h"
#include "rr/spu_tone_update.h"

s32 SsUtSetVagAtr(s32 a0, s32 a1, s32 a2, const u8 *a3) {
    s16 bank = a0;
    s16 prog = a1;
    s16 idx;
    const VagAtr *src = (const VagAtr *)a3;
    if (_svm_vab_used[bank] == 1) {
        _SsVmVSetUp(bank, prog);
        idx = a2 + _svm_cur.f07 * 16;
        ((VagAtr *)_svm_tn)[idx].f00 = src->f00;
        ((VagAtr *)_svm_tn)[idx].f01 = src->f01;
        ((VagAtr *)_svm_tn)[idx].f02 = src->f02;
        ((VagAtr *)_svm_tn)[idx].f03 = src->f03;
        ((VagAtr *)_svm_tn)[idx].f04 = src->f04;
        ((VagAtr *)_svm_tn)[idx].f05 = src->f05;
        ((VagAtr *)_svm_tn)[idx].f07 = src->f07;
        ((VagAtr *)_svm_tn)[idx].f06 = src->f06;
        ((VagAtr *)_svm_tn)[idx].f08 = src->f08;
        ((VagAtr *)_svm_tn)[idx].f09 = src->f09;
        ((VagAtr *)_svm_tn)[idx].f0A = src->f0A;
        ((VagAtr *)_svm_tn)[idx].f0B = src->f0B;
        ((VagAtr *)_svm_tn)[idx].f0C = src->f0C;
        ((VagAtr *)_svm_tn)[idx].f0D = src->f0D;
        ((VagAtr *)_svm_tn)[idx].f10 = src->f10;
        ((VagAtr *)_svm_tn)[idx].f12 = src->f12;
        ((VagAtr *)_svm_tn)[idx].f14 = src->f14;
        ((VagAtr *)_svm_tn)[idx].f16 = src->f16;
        return 0;
    }
    return -1;
}
