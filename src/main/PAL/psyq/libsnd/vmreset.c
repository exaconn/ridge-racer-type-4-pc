/* MASPSX_FLAGS: --assembler-profile=gnu */
/* CC1_FLAGS: -mno-split-addresses */
#include "types.h"
#include "psyq/libspu/libspu.h"
#include "psyq/libsnd/vmtick2.h"
#include "psyq/libspu/sminit.h"
#include "game/data/globals.h"



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
    u8 pad18[5];
    s8 f1D;
    s16 f1E;
    s16 f20;
    s16 f22;
    s16 f24;
    s16 f26;
    s16 f28;
    s16 f2A;
    s16 f2C;
    s16 f2E;
    s16 f30;
    s16 f32;
    s16 f34;
} SpuVoiceRow36;

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
} Unk80110008;

extern void func_80084674(SpuVoiceAttr *);
extern void func_80087DA4(s32);
extern void func_80088DA4(s32);

extern SpuVoiceRow36 D_8010F908[];
extern u8 D_8010FE18[];
extern u16 D_8010FE38[];
extern Unk80110008 D_80110008;
extern s16 D_801100A0;
extern s16 D_801100EA;
extern s8 D_801100FC;
extern s16 D_80110118;
extern u8 D_80110120[];
extern s16 D_80110178;
extern u8 D_801101C8[];

void func_800879F4(s32 arg0) {
    SpuVoiceAttr attr;
    u16 i;
    u16 *p;
    s32 v;
    s32 c18, cff, cm1, c40, c1;

    func_80087DA4(0);
    D_801100A0 = 0;
    SpuInitMalloc(0x20, D_801101C8);

    i = 0;
    p = D_8010FE38;
    do {
        p[i] = 0;
        i++;
    } while (i < 0xC0);

    for (i = 0; i < 0x18; i++) {
        D_8010FE18[i] = 0;
    }

    D_80110178 = 0;

    for (i = 0; i < 0x10; i++) {
        D_80110120[i] = 0;
    }

    v = (s8)arg0;
    if ((u32)v >= 0x18) {
        D_801100FC = 0x18;
    } else {
        D_801100FC = v;
    }

    attr.mask = 0x60093;
    i = 0;
    attr.pitch = 0x1000;
    attr.addr = 0x1000;
    attr.adsr1 = 0x80FF;
    attr.volume.left = 0;
    attr.volume.right = 0;
    attr.adsr2 = 0x4000;

    if (D_801100FC > 0) {
        c18 = 0x18;
        cff = 0xFF;
        cm1 = -1;
        c40 = 0x40;
        c1 = 1;
        do {
            D_8010F908[i].f02 = c18;
            D_8010F908[i].f00 = cff;
            D_8010F908[i].f1D = 0;
            D_8010F908[i].f04 = 0;
            D_8010F908[i].f06 = 0;
            D_8010F908[i].f10 = cm1;
            D_8010F908[i].f12 = 0;
            D_8010F908[i].f14 = 0;
            D_8010F908[i].f16 = cff;
            D_8010F908[i].f08 = 0;
            D_8010F908[i].f0C = 0;
            D_8010F908[i].f0A = c40;
            D_8010F908[i].f1E = 0;
            D_8010F908[i].f20 = 0;
            D_8010F908[i].f22 = 0;
            D_8010F908[i].f24 = 0;
            D_8010F908[i].f2A = 0;
            D_8010F908[i].f2C = 0;
            D_8010F908[i].f2E = 0;
            D_8010F908[i].f30 = 0;
            D_8010F908[i].f32 = 0;
            D_8010F908[i].f26 = 0;
            attr.voice = c1 << i;
            func_80084674(&attr);
            D_80110118 = i;
            func_80088DA4(1);
            i++;
        } while (i < D_801100FC);
    }

    D_80110008.unk0 = 0;
    D_80110008.unk8 = 0x3FFF;
    D_80110008.unkA = 0x3FFF;
    D_80110008.unk4 = 0;
    D_8010F8F8 = 0;
    D_8010F8FA = 0;
    D_80110000 = 0;
    D_8010F8FC = 0;
    D_8010F8FE = 0;
    D_8010F900 = 0;
    D_8010F902 = 0;
    D_80110130 = 0;
    D_801100E8 = 0;
    D_801100EA = 0x80;
    func_80087064();
}
