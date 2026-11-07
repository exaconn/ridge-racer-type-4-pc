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

typedef struct {
    u8 pad00[2];
    u8 unk02;
    u8 unk03;
    u8 pad04[0x20 - 4];
} SpuTone20;

typedef struct {
    u8 pad00[0x26];
    s8 unk26;
    u8 pad27[0x58 - 0x27];
    u16 value58;
    u16 value5A;
    u16 volume5C;
    u16 volume5E;
    s16 unk60[40];
} ChannelRecordB0;

typedef struct {
    s16 value00;
    u16 age02;
    u16 value04;
    u16 value06;
    s16 unk08;
    u8 unk0A;
    u8 pad0B;
    s16 unk0C;
    s16 group0E;
    s16 match10;
    s16 unk12;
    s16 match14;
    s16 unk16;
    s16 match18;
    s16 priority1A;
    u8 pad1C;
    u8 flags1D;
    u8 pad1E[0x36 - 0x1E];
} ControlRow36;

typedef struct {
    s16 left, right;
    u8 pad04[2];
    u16 program06, tone08, tone0A;
    u8 pad0C[4];
} SpuCachedVolume10;

extern ChannelRecordB0 *_ss_score[];

extern ControlRow36 _svm_voice[];

extern SpuCachedVolume10 _svm_sreg_buf[];

extern s16 _svm_stereo_mono;

extern s8 spuVmMaxVoice;

extern u32 D_800A48F8;

s16 _SsVmSetSeqVol(s16 a0, u16 a1, u16 a2) {
    s16 i;
    s32 v;
    s32 w;
    u32 vl;
    u32 sq;
    u32 vr;
    u16 right;
    u16 left;
    u8 pan;
    SpuTone20 *tone_ptr;
    ChannelRecordB0 *ptr;

    ptr = &_ss_score[a0 & 0xFF][(a0 & 0xFF00) >> 8];
    ptr->value58 = a1;
    ptr->value5A = a2;
    if (ptr->value58 >= 0x7F) {
        ptr->value58 = 0x7F;
    }
    if (ptr->value5A >= 0x7F) {
        ptr->value5A = 0x7F;
    }
    for (i = 0; i < spuVmMaxVoice; i++) {
        if (!(D_800A48F8 & (1 << i))) {
            if (_svm_voice[i].match10 == a0 && _svm_voice[i].match18 == ptr->unk26) {
                _SsVmVSetUp(_svm_voice[i].match18, _svm_voice[i].unk12);
                v = (_svm_voice[i].unk08 * ptr->unk60[_svm_voice[i].unk0C]) / 0x7F;
                w = v * 0x3FFF;
                vl = (_svm_vh[0x18] * w) / 0x3F01;
                tone_ptr = (SpuTone20 *)(((_svm_voice[i].unk12 << 4) + _svm_voice[i].unk16) * 32 + (s32)_svm_tn);
                vr = (vl * _svm_pg[_svm_voice[i].match14].value01 * tone_ptr->unk02) / 0x3F01;
                vl = (vr * ptr->value58) / 0x7F;
                vr = (vr * ptr->value5A) / 0x7F;
                pan = tone_ptr->unk03;
                if (pan < 0x40) {
                    vr = (vr * pan) / 0x3F;
                } else {
                    vl = (vl * (0x7F - pan)) / 0x3F;
                }
                right = vr;
                left = vl;
                pan = _svm_pg[_svm_voice[i].match14].value04;
                if (pan < 0x40) {
                    right = (right * pan) / 0x3F;
                } else {
                    left = (left * (0x7F - pan)) / 0x3F;
                }
                pan = _svm_voice[i].unk0A;
                if (pan < 0x40) {
                    s32 prod = right * pan;
                    /* ASM_BARRIER: signed high-multiply by 0x82082083 (/ 0x3F) in $s3 without emitting a second dead sra-31 sign pseudo in combine/reload */
                    __asm__("nop\n\tnop\n\tmult %0, $19\n\tmfhi $8\n\taddu %0, $8, %0" : "=r"(prod) : "0"(prod));
                    right = (u32)prod >> 5;
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
                *(s16 *)((s32)_svm_sreg_buf + i * 16 + 2) = sq;
                _svm_sreg_dirty[i] |= 3;
            }
        }
    }
    return a0;
}
