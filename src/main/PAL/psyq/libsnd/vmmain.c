/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libsnd/vmkoff.h"
#include "psyq/libsnd/vmnote2.h"
#include "psyq/libsnd/vmupd.h"
#include "psyq/libsnd/vmmain.h"
#include "rr/spu_voice_cache.h"



extern s32 _SsVmAlloc();
extern void vmNoiseOn();

s32 _SsVmKeyOn(s32 a0_in, s32 a1_in, s32 a2_in, s32 a3_in, s32 a4_in, s32 a5_in);

void _spu_setInTransfer(s32 mode) {
    if (mode == 1) {
        _spu_inTransfer = 0;
    } else {
        _spu_inTransfer = 1;
    }
}

s32 _spu_getInTransfer(void) {
    return _spu_inTransfer != 1;
}

s32 _SsVmKeyOn(s32 a0_in, s32 a1_in, s32 a2_in, s32 a3_in, s32 a4_in, s32 a5_in)
{
    short a0 = a0_in;
    short a1 = a1_in;
    short a2 = a2_in;
    short a3 = a3_in;
    unsigned char sp10[0x80];
    unsigned char sp90[0x80];
    unsigned char n;
    unsigned char i;
    short v;
    int ret;
    ChannelValueRecordB0 *s5;
    u16 toneIndex;
    ChannelValueRecordB0 **tbl;
    int a4 = (unsigned short)a4_in;
    int a5 = (unsigned short)a5_in;

    ret = 0;
    tbl = _ss_score;
    s5 = tbl[a0 & 0xFF] + ((a0 & 0xFF00) >> 8);
    if (_SsVmVSetUp(a1, a2) != 0) {
        return -1;
    }
    _svm_cur.f14 = a0;
    _svm_cur.f02 = a3;
    _svm_cur.f03 = 0;
    if (a0 == 0x21) {
        _svm_cur.f04 = a4;
    } else {
        /* ASM_BARRIER: pins the product to $v0; with the identical arms (merged by
           post-reload cross-jumping) this keeps mult and div in separate flow blocks,
           so the highpart stays in HI and is reloaded via $t0 like the original */
        register int t asm("$2") = a4 * s5->h60[s5->b17];
        if (t < 0) {
            _svm_cur.f04 = t / 127;
        } else {
            _svm_cur.f04 = t / 127;
        }
    }
    _svm_cur.f05 = a5;
    _svm_cur.f0A = _svm_pg[a2][VAB_PROGRAM_VOLUME];
    _svm_cur.f0B = _svm_pg[a2][VAB_PROGRAM_PAN];
    _svm_cur.f00 = _svm_pg[a2][VAB_PROGRAM_TONE_COUNT];
    if (_svm_cur.f07 >= ((VabHdr *)_svm_vh)->ps) {
        return -1;
    }
    if (a4 == 0) {
        ret = _SsVmKeyOff(a0, a1, a2, (unsigned short)a3);
    } else {
        n = _SsVmSelectToneAndVag(sp90, sp10);
        for (i = 0; i < n; i++) {
            _svm_cur.f16 = sp10[i];
            _svm_cur.f0C = sp90[i];
            toneIndex = (u16)(_svm_cur.f07 * 16 + _svm_cur.f0C);
            _svm_cur.f0F = _svm_tn[toneIndex][VAB_TONE_PRIORITY];
            _svm_cur.f0D = _svm_tn[toneIndex][VAB_TONE_VOLUME];
            _svm_cur.f0E = _svm_tn[toneIndex][VAB_TONE_PAN];
            _svm_cur.f10 = _svm_tn[toneIndex][VAB_TONE_CENTER];
            _svm_cur.f11 = _svm_tn[toneIndex][VAB_TONE_SHIFT];
            _svm_cur.f12 = _svm_tn[toneIndex][VAB_TONE_MODE];
            v = (unsigned char)_SsVmAlloc(0);
            _svm_cur.f18 = v;
            if (v < spuVmMaxVoice) {
                int a0_ext = a0;
                _svm_voice[_svm_cur.f18].flags1D = 1;
                _svm_voice[_svm_cur.f18].age02 = 0;
                _svm_voice[_svm_cur.f18].match10 = a0;
                _svm_voice[_svm_cur.f18].match18 = _svm_cur.f01;
                _svm_voice[_svm_cur.f18].unk12 = _svm_cur.f07;
                _svm_voice[_svm_cur.f18].match14 = a2;
                if (a0_ext != 0x21) {
                    _svm_voice[_svm_cur.f18].unk08 = a4;
                    _svm_voice[_svm_cur.f18].unk0C = s5->b17;
                }
                _svm_voice[_svm_cur.f18].unk0A = a5;
                _svm_voice[_svm_cur.f18].unk16 = _svm_cur.f0C;
                _svm_voice[_svm_cur.f18].group0E = a3;
                _svm_voice[_svm_cur.f18].priority1A = _svm_cur.f0F;
                _svm_voice[_svm_cur.f18].value00 = _svm_cur.f16;
                _SsVmDoAllocate();
                if (_svm_cur.f16 == 0xFF) {
                    vmNoiseOn((unsigned char)_svm_cur.f18);
                } else {
                    _SsVmKeyOnNow(n, note2pitch());
                }
                ret |= 1 << _svm_cur.f18;
            } else {
                ret = -1;
            }
        }
    }
    return ret;
}
