/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"

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

typedef struct {
    u8 f00, f01, f02, f03, f04, pad05[11];
} ProgAtr;

typedef struct {
    u8 f00, f01, f02, f03, f04, f05;
    u8 pad06[0x16 - 0x06];
    u16 f16;
    u8 pad18[0x20 - 0x18];
} VagAtr;

typedef struct {
    s16 f00;
    s16 f02;
    s16 f04;
    s16 f06;
    s16 f08;
    s8 f0A;
    s8 f0B;
    s16 f0C;
    s16 f0E;
    s16 f10;
    s16 f12;
    s16 f14;
    s16 f16;
    s16 f18;
    s16 f1A;
    s8 f1C;
    s8 f1D;
    u8 pad1E[0x36 - 0x1E];
} SpuVoice;

extern s32 D_8010F05C;
extern SpuVoice D_8010F908[];
extern ProgAtr *D_801100EC;
extern VagAtr *D_801100F8;
extern SvmCur D_80110100;

extern s32 func_8008A564(s16, s16);
extern void func_80086E74(void);
extern void func_80088794(u8);
extern u16 func_80088608(s32, s32);
extern void func_80088E74(u8, u16);

s16 func_800857A4(s16 arg0, s16 arg1, s16 arg2, s16 arg3, u16 arg4_in, u16 arg5_in, s16 arg6, s16 arg7) {
    s16 tone_idx;
    VagAtr *vag;
    s32 temp_a0;
    s32 arg4 = arg4_in;
    s32 arg5 = arg5_in;

    if (D_8010F05C == 1) {
        return -1;
    }
    D_8010F05C = 1;
    if (((u16)arg0 < 0x18) && (func_8008A564(arg1, arg2) == 0)) {
        D_80110100.f14 = 0x21;
        D_80110100.f02 = arg4;
        D_80110100.f03 = arg5;
        D_80110100.f0C = arg3;
        if (arg6 == arg7) {
            D_80110100.f05 = 0x40;
            D_80110100.f04 = arg6;
        } else if (arg6 > arg7) {
            D_80110100.f04 = arg6;
            D_80110100.f05 = (arg7 << 6) / arg6;
        } else {
            D_80110100.f04 = arg7;
            D_80110100.f05 = 0x7F - ((arg6 << 6) / arg7);
        }
        D_80110100.f0A = D_801100EC[arg2].f01;
        D_80110100.f0B = D_801100EC[arg2].f04;
        D_80110100.f00 = D_801100EC[arg2].f00;
        tone_idx = D_80110100.f07 * 16 + D_80110100.f0C;
        vag = (VagAtr *)(tone_idx * 32 + (s32)D_801100F8);
        D_80110100.f0F = vag->f00;
        temp_a0 = vag->f16;
        D_80110100.f16 = temp_a0;
        D_80110100.f0D = vag->f02;
        D_80110100.f0E = vag->f03;
        D_80110100.f10 = vag->f04;
        D_80110100.f11 = vag->f05;
        D_80110100.f12 = vag->f01;
        if (temp_a0 == 0) {
            D_8010F05C = 0;
            return -1;
        }
    } else {
        D_8010F05C = 0;
        return -1;
    }
    D_80110100.f18 = arg0;
    D_8010F908[arg0].f10 = 0x21;
    D_8010F908[arg0].f18 = arg1;
    D_8010F908[arg0].f12 = D_80110100.f07;
    D_8010F908[arg0].f14 = arg2;
    D_8010F908[arg0].f00 = D_80110100.f16;
    D_8010F908[arg0].f16 = D_80110100.f0C;
    D_8010F908[arg0].f0E = arg4;
    D_8010F908[arg0].f1D = 1;
    D_8010F908[arg0].f02 = 0;
    func_80086E74();
    if (D_80110100.f16 == 0xFF) {
        func_80088794(arg0);
    } else {
        func_80088E74(1, func_80088608(arg4, arg5));
    }
    D_8010F05C = 0;
    return arg0;
}
