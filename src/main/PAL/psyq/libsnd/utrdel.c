/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/library.h"
#include "psyq/libsnd/utrev.h"
#include "psyq/libspu/srevattr.h"

extern int _svm_rattr[];
extern s16 D_8011000C;

s16 SsUtGetReverbType(void);

s32 SsUtSetReverbType(s32 raw_a0) {
    s32 neg = 0;
    s16 a0 = raw_a0;
    if (a0 < 0) {
        neg = 1;
        a0 = -(u16)a0;
    }
    if ((u16)a0 < 10) {
        _svm_rattr[0] = 1;
        if (neg) _svm_rattr[1] = a0 | 0x100;
        else _svm_rattr[1] = a0;
        if (a0 == 0) SpuSetReverb(0);
        SpuSetReverbModeParam((void *)_svm_rattr);
        return a0;
    }
    return -1;
}

s16 SsUtGetReverbType(void) {
    return D_8011000C;
}
