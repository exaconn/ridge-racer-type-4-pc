/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "rr/channels.h"
#include "rr/spu.h"

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

extern u16 D_8010F8F8;
extern u16 D_8010F8FA;
extern u16 D_8010F8FC;
extern u16 D_8010F8FE;
extern u16 D_8010F900;
extern u16 D_8010F902;
extern u16 D_8010F90C[];
extern s16 D_8010FE38[];
extern s16 D_8010FE3A[];
extern s16 D_8010FE3C[];
extern u16 D_80110000;
extern u16 D_80110002;
extern s16 D_801100E8;
extern SvmCur D_80110100;

void func_80088E74(s16 arg0, s16 arg1) {
    u32 vol;
    u32 a3;
    u32 left, right;
    u16 voice_off;
    s32 v;
    RrChannelValueRecordB0 *rec;
    u8 pan;
    u16 low_mask, high_mask;

    v = D_80110100.f18;
    voice_off = v << 3;
    a3 = ((D_80110100.f04 * 0x3FFF) * D_801100F4[0x18]) / 16129;
    vol = (a3 * D_80110100.f0A * D_80110100.f0D) / 16129U;
    rec = D_8010F068[(u16)D_80110100.f14 & 0xFF] + ((D_80110100.f14 & 0xFF00) >> 8);
    a3 = vol;
    if (D_80110100.f14 != 0x21) {
        /* ASM_BARRIER: prevent replacing vol with a3 in first multiply */
        __asm__("" : "+r"(vol));
        a3 = (vol * rec->value58) / 127U;
        vol = (vol * rec->value5A) / 127U;
    }
    pan = D_80110100.f0E;
    if (pan < 0x40U) {
        left = a3;
        right = (vol * pan) / 63U;
    } else {
        right = vol;
        left = (a3 * (0x7F - pan)) / 63U;
    }
    pan = D_80110100.f0B;
    if (pan < 0x40U) {
        right = (right * pan) / 63U;
    } else {
        left = (left * (0x7F - pan)) / 63U;
    }
    pan = D_80110100.f05;
    if (pan < 0x40U) {
        right = (right * pan) / 63U;
    } else {
        left = (left * (0x7F - pan)) / 63U;
    }
    if (D_801100E8 == 1) {
        if (left < right) {
            left = right;
        } else {
            right = left;
        }
    }
    if (D_80110100.f14 != 0x21) {
        left = (left * left) / 16383U;
        right = (right * right) / 16383U;
    }
    D_8010FE3C[voice_off] = arg1;
    D_8010FE38[voice_off] = left;
    D_8010FE3A[voice_off] = right;
    ((u8 *)&D_8010FE18)[D_80110100.f18] |= 7;
    D_8010F90C[D_80110100.f18 * 27] = arg1;
    if (D_80110100.f18 < 0x10) {
        low_mask = 1 << D_80110100.f18;
        high_mask = 0;
    } else {
        low_mask = 0;
        high_mask = 1 << (D_80110100.f18 - 0x10);
    }
    if (D_80110100.f12 & 4) {
        D_8010F8FC |= low_mask;
        D_8010F8FE |= high_mask;
    } else {
        D_8010F8FC &= ~low_mask;
        D_8010F8FE &= ~high_mask;
    }
    D_8010F900 &= ~low_mask;
    D_8010F902 &= ~high_mask;
    D_8010F8FA |= high_mask;
    D_8010F8F8 |= low_mask;
    D_80110000 &= ~D_8010F8F8;
    D_80110002 &= ~D_8010F8FA;
}
