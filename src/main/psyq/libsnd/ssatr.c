#include "psyq/libsnd/vmupd.h"
#include "psyq/libsnd/ssatr.h"
#include "rr/channels.h"
#include "rr/spu.h"

int _SsVmPitchBend(s32 raw_a0, s32 raw_a1, s32 raw_a2, s32 raw_a3) {
    s16 a0 = raw_a0;
    s16 a1 = raw_a1;
    s16 a2 = raw_a2;
    u16 a3 = raw_a3;
    s16 i;
    s32 s;
    _SsVmVSetUp(a1, a2);
    _svm_cur_vab_id = a0;
    s = 0;
    for (i = 0; i < spuVmMaxVoice; i++) {
        s += _SsVmPBVoice(i, a0, a1, a2, a3);
    }
    return s;
}
