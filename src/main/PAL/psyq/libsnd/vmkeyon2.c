/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "psyq/libsnd/libsnd.h"
#include "psyq/libspu/libspu.h"
#include "psyq/libsnd/vmnote2.h"
#include "psyq/libsnd/vmupd.h"

typedef struct {
    s8 f00, f01, f02, f03, f04;
    u8 f05;
    s8 f06, f07, f08, f09, f0A;
    u8 f0B;
    s8 f0C, f0D;
    u8 f0E;
    s8 f0F, f10, f11;
    u8 f12;
    s8 f13;
    s16 f14, f16, f18;
} SvmCur;




extern s32 _snd_ev_flag;
extern SpuVoice _svm_voice[];
extern ProgAtr *_svm_pg;
extern VagAtr *_svm_tn;
extern SvmCur _svm_cur;

extern s32 _SsVmVSetUp(s16, s16);
extern void _SsVmDoAllocate(void);
extern void vmNoiseOn(u8);

int SsUtKeyOnV(s16 arg0, s16 arg1, s16 arg2, s16 arg3, u16 arg4_in, u16 arg5_in, s16 arg6, s16 arg7) {
    s16 tone_idx;
    VagAtr *vag;
    s32 temp_a0;
    s32 arg4 = arg4_in;
    s32 arg5 = arg5_in;

    if (_snd_ev_flag == 1) {
        return -1;
    }
    _snd_ev_flag = 1;
    if (((u16)arg0 < 0x18) && (_SsVmVSetUp(arg1, arg2) == 0)) {
        _svm_cur.f14 = 0x21;
        _svm_cur.f02 = arg4;
        _svm_cur.f03 = arg5;
        _svm_cur.f0C = arg3;
        if (arg6 == arg7) {
            _svm_cur.f05 = 0x40;
            _svm_cur.f04 = arg6;
        } else if (arg6 > arg7) {
            _svm_cur.f04 = arg6;
            _svm_cur.f05 = (arg7 << 6) / arg6;
        } else {
            _svm_cur.f04 = arg7;
            _svm_cur.f05 = 0x7F - ((arg6 << 6) / arg7);
        }
        _svm_cur.f0A = _svm_pg[arg2].f01;
        _svm_cur.f0B = _svm_pg[arg2].f04;
        _svm_cur.f00 = _svm_pg[arg2].f00;
        tone_idx = _svm_cur.f07 * 16 + _svm_cur.f0C;
        vag = (VagAtr *)(tone_idx * 32 + (s32)_svm_tn);
        _svm_cur.f0F = vag->f00;
        temp_a0 = vag->f16;
        _svm_cur.f16 = temp_a0;
        _svm_cur.f0D = vag->f02;
        _svm_cur.f0E = vag->f03;
        _svm_cur.f10 = vag->f04;
        _svm_cur.f11 = vag->f05;
        _svm_cur.f12 = vag->f01;
        if (temp_a0 == 0) {
            _snd_ev_flag = 0;
            return -1;
        }
    } else {
        _snd_ev_flag = 0;
        return -1;
    }
    _svm_cur.f18 = arg0;
    _svm_voice[arg0].f10 = 0x21;
    _svm_voice[arg0].f18 = arg1;
    _svm_voice[arg0].f12 = _svm_cur.f07;
    _svm_voice[arg0].f14 = arg2;
    _svm_voice[arg0].f00 = _svm_cur.f16;
    _svm_voice[arg0].f16 = _svm_cur.f0C;
    _svm_voice[arg0].f0E = arg4;
    _svm_voice[arg0].f1D = 1;
    _svm_voice[arg0].f02 = 0;
    _SsVmDoAllocate();
    if (_svm_cur.f16 == 0xFF) {
        vmNoiseOn(arg0);
    } else {
        _SsVmKeyOnNow(1, note2pitch2(arg4, arg5));
    }
    _snd_ev_flag = 0;
    return arg0;
}
