/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libsnd/libsnd.h"
#include "psyq/libspu/libspu.h"
#include "psyq/libsnd/vmnote2.h"
#include "psyq/libsnd/vmupd.h"
#include "rr/spu_voice_cache.h"

extern s32 _SsVmAlloc(s16);
extern void vmNoiseOn(u8);

s32 SsUtKeyOn(s16 arg1, s16 arg2, s16 arg3, s16 arg4_in, u16 arg5_in, s16 arg6, s16 arg7) {
    s16 tone_idx;
    s16 temp_a0;
    s16 voice;
    s32 arg5 = arg5_in;

    if (_snd_ev_flag == 1) {
        return -1;
    }
    _snd_ev_flag = 1;
    if (_SsVmVSetUp(arg1, arg2) == 0) {
        _svm_cur.f14 = 0x21;
        _svm_cur.f02 = arg4_in;
        _svm_cur.f03 = arg5;
        _svm_cur.f0C = arg3;
        if (arg6 == arg7) {
            _svm_cur.f05 = 0x40;
            _svm_cur.f04 = arg6;
        } else if (arg6 > arg7) {
            _svm_cur.f04 = arg6;
            _svm_cur.f05 = (arg7 * 64) / arg6;
        } else {
            _svm_cur.f04 = arg7;
            _svm_cur.f05 = 0x7F - ((arg6 * 64) / arg7);
        }
        _svm_cur.f0A = _svm_pg[arg2][VAB_PROGRAM_VOLUME];
        _svm_cur.f0B = _svm_pg[arg2][VAB_PROGRAM_PAN];
        _svm_cur.f00 = _svm_pg[arg2][VAB_PROGRAM_TONE_COUNT];
        tone_idx = _svm_cur.f07 * 16 + _svm_cur.f0C;
        _svm_cur.f0F = _svm_tn[tone_idx][VAB_TONE_PRIORITY];
        temp_a0 = *(u16 *)&_svm_tn[tone_idx][VAB_TONE_SAMPLE];
        _svm_cur.f16 = temp_a0;
        _svm_cur.f0D = _svm_tn[tone_idx][VAB_TONE_VOLUME];
        _svm_cur.f0E = _svm_tn[tone_idx][VAB_TONE_PAN];
        _svm_cur.f10 = _svm_tn[tone_idx][VAB_TONE_CENTER];
        _svm_cur.f11 = _svm_tn[tone_idx][VAB_TONE_SHIFT];
        _svm_cur.f12 = _svm_tn[tone_idx][VAB_TONE_MODE];
        if (temp_a0 == 0) {
            _snd_ev_flag = 0;
            return -1;
        }
    } else {
        _snd_ev_flag = 0;
        return -1;
    }
    voice = (u8)_SsVmAlloc(temp_a0);
    if (voice == spuVmMaxVoice) {
        _snd_ev_flag = 0;
        return -1;
    }
    _svm_cur.f18 = voice;
    _svm_voice[voice].match10 = 0x21;
    _svm_voice[voice].match18 = arg1;
    _svm_voice[voice].unk12 = _svm_cur.f07;
    _svm_voice[voice].match14 = arg2;
    _svm_voice[voice].value00 = _svm_cur.f16;
    _svm_voice[voice].unk16 = _svm_cur.f0C;
    _svm_voice[voice].group0E = arg4_in;
    _svm_voice[voice].flags1D = 1;
    _svm_voice[voice].age02 = 0;
    _SsVmDoAllocate();
    if (_svm_cur.f16 == 0xFF) {
        vmNoiseOn(voice);
    } else {
        _SsVmKeyOnNow(1, note2pitch2((u16)arg4_in, arg5));
    }
    _snd_ev_flag = 0;
    return voice;
}
