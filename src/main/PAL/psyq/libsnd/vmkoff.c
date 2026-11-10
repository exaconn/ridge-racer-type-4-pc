/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libsnd/vmkoff.h"

void _SsVmSeqKeyOff(s32 input);

void _SsVmSeqKeyOff(s32 input) {
    s16 wanted = input;
    u8 i;
    for (i = 0; i < spuVmMaxVoice; i++) {
        if ((_snd_vmask & (1 << i)) == 0) {
            if (_svm_voice[i].match10 == wanted) {
                _svm_cur.f18 = i;
                _SsVmKeyOffNow(0);
            }
        }
    }
}

unsigned char _SsVmSelectToneAndVag(u8 *a0, u8 *a1) {
    u8 n = 0;
    s8 i = 0;
    if (_svm_cur.f00 > 0) {
        do {
            int ie = i;
            u8 *t = (u8 *)(((_svm_cur.f07 << 4) + ie) * 32 + (int)_svm_tn);
            if (!(t[6] > _svm_cur.f02) && !(t[7] < _svm_cur.f02)) {
                a1[n] = t[0x16];
                a0[n++] = i;
            }
            i++;
        } while (i < _svm_cur.f00);
    }
    return n;
}
