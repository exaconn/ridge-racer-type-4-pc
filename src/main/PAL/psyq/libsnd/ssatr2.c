/* MASPSX_FLAGS: --assembler-profile=gnu */
/* GCC_VERSION: 2.7.2 */
/* CC1_FLAGS: -mhard-float */
#include "types.h"
#include "rr/spu.h"
#include "rr/channel_stream.h"
#include "game/data/globals.h"

typedef struct {
    u8 value00, value01, value02, value03, value04, pad05;
    u16 value06;
    u8 value08, pad09[3];
    u16 value0C, value0E;
} SpuProgram10;

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

extern ControlRow36 D_8010F908[];
extern SpuCachedVolume10 D_8010FE38[];
extern s8 D_801100FC;
extern u32 D_800A48F8;

s32 func_80089624(s16 bank, s16 program, s32 value);
s32 func_80089694(s16 bank, s16 program);
s32 func_800896E4(s16 bank, s16 program, s32 value);
s32 func_80089754(s16 bank, s16 program);
s16 func_800897A4(s16 a0, u16 a1, u16 a2);

s32 func_80089624(s16 bank, s16 program, s32 value) {
    if(func_8008A564(bank, program)!=0)return -1;
    D_801100EC[program].value01=value;
    return D_801100EC[program].value01;
}

s32 func_80089694(s16 bank, s16 program) {
    if(func_8008A564(bank, program)!=0)return -1;
    return D_801100EC[program].value01;
}

s32 func_800896E4(s16 bank, s16 program, s32 value) {
    if(func_8008A564(bank, program)!=0)return -1;
    D_801100EC[program].value04=value;
    return D_801100EC[program].value04;
}

s32 func_80089754(s16 bank, s16 program) {
    if(func_8008A564(bank, program)!=0)return -1;
    return D_801100EC[program].value04;
}

/* legacy typing of shared globals in this part of the unit */
#define D_8010FE18 (*(u8 (*)[])&D_8010FE18)
#define D_801100EC ((SpuProgram10 *)D_801100EC)
#define D_801100F8 ((SpuTone20 *)D_801100F8)
#define func_8008A564 ((s32 (*)(s16 bank, s16 program))func_8008A564)

s16 func_800897A4(s16 a0, u16 a1, u16 a2) {
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

    ptr = &D_8010F068[a0 & 0xFF][(a0 & 0xFF00) >> 8];
    ptr->value58 = a1;
    ptr->value5A = a2;
    if (ptr->value58 >= 0x7F) {
        ptr->value58 = 0x7F;
    }
    if (ptr->value5A >= 0x7F) {
        ptr->value5A = 0x7F;
    }
    for (i = 0; i < D_801100FC; i++) {
        if (!(D_800A48F8 & (1 << i))) {
            if (D_8010F908[i].match10 == a0 && D_8010F908[i].match18 == ptr->unk26) {
                func_8008A564(D_8010F908[i].match18, D_8010F908[i].unk12);
                v = (D_8010F908[i].unk08 * ptr->unk60[D_8010F908[i].unk0C]) / 0x7F;
                w = v * 0x3FFF;
                vl = (D_801100F4[0x18] * w) / 0x3F01;
                tone_ptr = (SpuTone20 *)(((D_8010F908[i].unk12 << 4) + D_8010F908[i].unk16) * 32 + (s32)D_801100F8);
                vr = (vl * D_801100EC[D_8010F908[i].match14].value01 * tone_ptr->unk02) / 0x3F01;
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
                pan = D_801100EC[D_8010F908[i].match14].value04;
                if (pan < 0x40) {
                    right = (right * pan) / 0x3F;
                } else {
                    left = (left * (0x7F - pan)) / 0x3F;
                }
                pan = D_8010F908[i].unk0A;
                if (pan < 0x40) {
                    s32 prod = right * pan;
                    /* ASM_BARRIER: signed high-multiply by 0x82082083 (/ 0x3F) in $s3 without emitting a second dead sra-31 sign pseudo in combine/reload */
                    __asm__("nop\n\tnop\n\tmult %0, $19\n\tmfhi $8\n\taddu %0, $8, %0" : "=r"(prod) : "0"(prod));
                    right = (u32)prod >> 5;
                } else {
                    left = (left * (0x7F - pan)) / 0x3F;
                }
                if (D_801100E8 == 1) {
                    if (left < right) {
                        left = right;
                    } else {
                        right = left;
                    }
                }
                v = D_8010FE38[i].left = (left * left) / 0x3FFF;
                sq = (right * right) / 0x3FFF;
                *(s16 *)((s32)D_8010FE38 + i * 16 + 2) = sq;
                D_8010FE18[i] |= 3;
            }
        }
    }
    return a0;
}

#undef D_8010FE18
#undef D_801100EC
#undef D_801100F8
#undef func_8008A564
