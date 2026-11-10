/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libsnd/vmnote2.h"
#include "psyq/libsnd/vmupd.h"
#include "game/data/globals.h"

extern u16 D_8010F90C[];
extern s16 D_8010FE3A[];
extern s16 D_8010FE3C[];

void _SsVmKeyOnNow(u8 arg0, u16 arg1);
s16 _SsVmPBVoice(s32 a0_in, s32 a1_in, s32 a2_in, s32 a3_in, s32 a4);

void _SsVmKeyOnNow(u8 arg0, u16 arg1) {
    u32 vol;
    u32 a3;
    u32 left, right;
    u16 voice_off;
    s32 v;
    ChannelValueRecordB0 *rec;
    u8 pan;
    u16 low_mask, high_mask;

    v = _svm_cur.f18;
    voice_off = v << 3;
    a3 = ((_svm_cur.f04 * 0x3FFF) * _svm_vh[0x18]) / 16129;
    vol = (a3 * _svm_cur.f0A * _svm_cur.f0D) / 16129U;
    rec = _ss_score[(u16)_svm_cur.f14 & 0xFF] + ((_svm_cur.f14 & 0xFF00) >> 8);
    a3 = vol;
    if (_svm_cur.f14 != 0x21) {
        /* ASM_BARRIER: prevent replacing vol with a3 in first multiply */
        __asm__("" : "+r"(vol));
        a3 = (vol * rec->value58) / 127U;
        vol = (vol * rec->value5A) / 127U;
    }
    pan = _svm_cur.f0E;
    if (pan < 0x40U) {
        left = a3;
        right = (vol * pan) / 63U;
    } else {
        right = vol;
        left = (a3 * (0x7F - pan)) / 63U;
    }
    pan = _svm_cur.f0B;
    if (pan < 0x40U) {
        right = (right * pan) / 63U;
    } else {
        left = (left * (0x7F - pan)) / 63U;
    }
    pan = _svm_cur.f05;
    if (pan < 0x40U) {
        right = (right * pan) / 63U;
    } else {
        left = (left * (0x7F - pan)) / 63U;
    }
    if (_svm_stereo_mono == 1) {
        if (left < right) {
            left = right;
        } else {
            right = left;
        }
    }
    if (_svm_cur.f14 != 0x21) {
        left = (left * left) / 16383U;
        right = (right * right) / 16383U;
    }
    D_8010FE3C[voice_off] = arg1;
    ((s16 *)_svm_sreg_buf)[voice_off] = left;
    D_8010FE3A[voice_off] = right;
    _svm_sreg_dirty.flags[_svm_cur.f18] |= 7;
    D_8010F90C[_svm_cur.f18 * 27] = arg1;
    if (_svm_cur.f18 < 0x10) {
        low_mask = 1 << _svm_cur.f18;
        high_mask = 0;
    } else {
        low_mask = 0;
        high_mask = 1 << (_svm_cur.f18 - 0x10);
    }
    if (_svm_cur.f12 & 4) {
        _svm_orev1 |= low_mask;
        _svm_orev2 |= high_mask;
    } else {
        _svm_orev1 &= ~low_mask;
        _svm_orev2 &= ~high_mask;
    }
    _svm_onos1 &= ~low_mask;
    _svm_onos2 &= ~high_mask;
    _svm_okon2 |= high_mask;
    _svm_okon1 |= low_mask;
    _svm_okof1 &= ~_svm_okon1;
    _svm_okof2 &= ~_svm_okon2;
}

typedef struct {
    u8 pad00[0xC];
    u8 f0C;
    u8 f0D;
    u8 pad0E[0x20 - 0xE];
} SvmTonePB;

s16 _SsVmPBVoice(s32 a0_in, s32 a1_in, s32 a2_in, s32 a3_in, s32 a4) {
    s16 a0 = a0_in;
    s16 a1 = a1_in;
    s16 a2 = a2_in;
    s16 a3 = a3_in;
    s16 t0;
    u16 s;
    s32 e;
    u16 idx;
    t0 = a4 - 0x40;
    if (_svm_voice[a0].match10 == a1) {
        if (_svm_voice[a0].match18 == a2) {
            if (_svm_voice[a0].match14 == a3) {
                s32 v0 = _svm_cur.f07;
                idx = _svm_voice[a0].unk16;
                s = _svm_voice[a0].group0E;
                idx += v0 * 16;
                if (t0 > 0) {
                    s32 x1 = t0 * ((SvmTonePB *)_svm_tn)[idx].f0D;
                    s += x1 / 63;
                    e = (x1 - (x1 / 63) * 63) * 2;
                } else if (t0 < 0) {
                    s32 x2 = t0 * ((SvmTonePB *)_svm_tn)[idx].f0C;
                    s = s + (x2 / 64) - 1;
                    v0 = (x2 - ((x2 / 64) << 6)) * 2;
                    e = v0 + 0x7F;
                } else {
                    e = 0;
                }
                _svm_cur.f0C = _svm_voice[a0].unk16;
                _svm_cur.f18 = a0;
                *(s16 *)_svm_sreg_buf[a0].pad04 = note2pitch2(s, e & 0xFFFF);
                _svm_sreg_dirty.flags[a0] |= 4;
                return 1;
            }
        }
    }
    return 0;
}
