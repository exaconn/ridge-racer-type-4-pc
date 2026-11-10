/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libsnd/vmsel.h"
#include "game/data/globals.h"

s32 _SsVmVSetUp(s32 bank, s32 program);

s32 _SsVmVSetUp(s32 bank, s32 program) {
    s16 i = bank;
    s16 j = program;
    if ((u16)i >= 16) {
        goto fail;
    }
    if (_svm_vab_used[i] != 1) {
        return -1;
    }
    if (j < kMaxPrograms) {
        goto publish;
    }
fail:
    return -1;
publish:
    _svm_vh = _svm_vab_vh[i];
    _svm_pg = _svm_vab_pg[i];
    _svm_tn = _svm_vab_tn[i];
    _svm_cur.f01 = i;
    _svm_cur.f06 = j;
    _svm_cur.f07 = _svm_pg[j].value08;
    return 0;
}

void SsSetMono(void) {
    _svm_stereo_mono = 1;
}

void SsSetStereo(void) {
    _svm_stereo_mono = 0;
}
