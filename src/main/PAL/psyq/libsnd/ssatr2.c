/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */
/* CC1_FLAGS: -mhard-float */
#include "types.h"
#include "rr/channels.h"
#include "rr/spu.h"
#include "psyq/libsnd/ssatr2.h"
#include "game/data/globals.h"


s32 _SsVmSetProgVol(s16 bank, s16 program, s32 value);
s32 _SsVmGetProgVol(s16 bank, s16 program);
s32 _SsVmSetProgPan(s16 bank, s16 program, s32 value);
s32 _SsVmGetProgPan(s16 bank, s16 program);
s16 _SsVmSetSeqVol(s32 a0_in, s32 a1_in, s32 a2_in, s32 a3_in);

s32 _SsVmSetProgVol(s16 bank, s16 program, s32 value) {
    if(_SsVmVSetUp(bank, program)!=0)return -1;
    _svm_pg[program][VAB_PROGRAM_VOLUME]=value;
    return _svm_pg[program][VAB_PROGRAM_VOLUME];
}

s32 _SsVmGetProgVol(s16 bank, s16 program) {
    if(_SsVmVSetUp(bank, program)!=0)return -1;
    return _svm_pg[program][VAB_PROGRAM_VOLUME];
}

s32 _SsVmSetProgPan(s16 bank, s16 program, s32 value) {
    if(_SsVmVSetUp(bank, program)!=0)return -1;
    _svm_pg[program][VAB_PROGRAM_PAN]=value;
    return _svm_pg[program][VAB_PROGRAM_PAN];
}

s32 _SsVmGetProgPan(s16 bank, s16 program) {
    if(_SsVmVSetUp(bank, program)!=0)return -1;
    return _svm_pg[program][VAB_PROGRAM_PAN];
}

s16 _SsVmSetSeqVol(s32 a0_in, s32 a1_in, s32 a2_in, s32 a3_in) {
    s16 a0 = a0_in;
    u16 a1 = a1_in;
    u16 a2 = a2_in;
    s16 i;
    s32 v;
    s32 w;
    u32 vl;
    u32 sq;
    u32 vr;
    u16 right;
    u16 left;
    u8 pan;
    s32 toneIndex;
    ChannelValueRecordB0 *ptr;

    ptr = &_ss_score[a0 & 0xFF][(a0 & 0xFF00) >> 8];
    ptr->h58[0] = a1;
    ptr->h58[1] = a2;
    if (ptr->h58[0] >= 0x7F) {
        ptr->h58[0] = 0x7F;
    }
    if (ptr->h58[1] >= 0x7F) {
        ptr->h58[1] = 0x7F;
    }
    for (i = 0; i < spuVmMaxVoice; i++) {
        if (!(_snd_vmask & (1 << i))) {
            if (_svm_voice[i].match10 == a0 && _svm_voice[i].match18 == ptr->b26) {
                _SsVmVSetUp(_svm_voice[i].match18, _svm_voice[i].unk12);
                v = (_svm_voice[i].unk08 * ptr->h60[_svm_voice[i].unk0C]) / 0x7F;
                w = v * 0x3FFF;
                vl = (_svm_vh[0x18] * w) / 0x3F01;
                toneIndex = _svm_voice[i].unk12 * 16 + _svm_voice[i].unk16;
                vr = (vl * _svm_pg[_svm_voice[i].match14][VAB_PROGRAM_VOLUME] * _svm_tn[toneIndex][VAB_TONE_VOLUME]) / 0x3F01;
                vl = (vr * ptr->h58[0]) / 0x7F;
                vr = (vr * ptr->h58[1]) / 0x7F;
                pan = _svm_tn[toneIndex][VAB_TONE_PAN];
                if (pan < 0x40) {
                    vr = (vr * pan) / 0x3F;
                } else {
                    vl = (vl * (0x7F - pan)) / 0x3F;
                }
                right = vr;
                left = vl;
                pan = _svm_pg[_svm_voice[i].match14][VAB_PROGRAM_PAN];
                if (pan < 0x40) {
                    right = (right * pan) / 0x3F;
                } else {
                    left = (left * (0x7F - pan)) / 0x3F;
                }
                pan = _svm_voice[i].unk0A;
                if (pan < 0x40) {
                    s32 prod = right * pan;
                    /* ASM_BARRIER: signed high-multiply by 0x82082083 (/ 0x3F) in $s3.
                     * This is reconstruction debt, not hardware transport: keep it
                     * visible here until replaced with matching C (docs/BARRIERS.md). */
                    __asm__("nop\n\tnop\n\tmult %0, $19\n\tmfhi $8\n\taddu %0, $8, %0" : "=r"(prod) : "0"(prod));
                    right = prod >> 5;
                } else {
                    left = (left * (0x7F - pan)) / 0x3F;
                }
                if (_svm_stereo_mono == 1) {
                    if (left < right) {
                        left = right;
                    } else {
                        right = left;
                    }
                }
                v = _svm_sreg_buf[i].left = (left * left) / 0x3FFF;
                sq = (right * right) / 0x3FFF;
                ((s16 *)((s32)_svm_sreg_buf + i * 16))[1] = sq;
                _svm_sreg_dirty.flags[i] |= 3;
            }
        }
    }
    return a0;
}
