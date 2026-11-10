/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu.h"

s32 SsUtGetProgAtr(s32 raw_bank, s16 program, u8 *output) {
    s16 bank = raw_bank;
    if (_svm_vab_used[bank] == 1) {
        _SsVmVSetUp(bank, program);
        output[0] = _svm_pg[program][VAB_PROGRAM_TONE_COUNT];
        output[1] = _svm_pg[program][VAB_PROGRAM_VOLUME];
        output[2] = _svm_pg[program][VAB_PROGRAM_PRIORITY];
        output[3] = _svm_pg[program][VAB_PROGRAM_MODE];
        output[4] = _svm_pg[program][VAB_PROGRAM_PAN];
        ((u16 *)output)[3] = *(u16 *)&_svm_pg[program][VAB_PROGRAM_ATTRIBUTE_FLAGS];
        return 0;
    }
    return -1;
}
