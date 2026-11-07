/* MASPSX_FLAGS: --assembler-profile=gnu */

/* GCC_VERSION: 2.7.2 */

/* CC1_FLAGS: -mhard-float */

#include "rr/spu.h"

/* legacy typing of shared globals in this part of the unit */
#define _svm_sreg_dirty (*(u8 (*)[])&_svm_sreg_dirty)

#define _svm_pg ((SpuProgram10 *)_svm_pg)

#define _svm_tn ((SpuTone20 *)_svm_tn)

#define _SsVmVSetUp ((s32 (*)(s16 bank, s16 program))_SsVmVSetUp)

#undef _svm_sreg_dirty

#undef _svm_pg

#undef _svm_tn

#undef _SsVmVSetUp

s32 _SsVmSetProgPan(s16 bank, s16 program, s32 value) {
    if(_SsVmVSetUp(bank, program)!=0)return -1;
    _svm_pg[program].value04=value;
    return _svm_pg[program].value04;
}
