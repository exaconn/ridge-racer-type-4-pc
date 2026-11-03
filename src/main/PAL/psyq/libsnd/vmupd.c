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
extern short func_80088608();
extern char D_8010F916[];
extern char D_8010F918[];
extern char D_8010F91C[];
extern char D_8010F91E[];
extern char D_8010F920[];
extern signed char D_80110107;
extern char D_8011010C[];

void func_80088E74(s16 arg0, s16 arg1);
int func_80089344(short a0, short a1, short a2, short a3, int a4);

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

/* legacy typing of shared globals in this part of the unit */
#define D_8010FE18 (*(char (*)[])&D_8010FE18)
#define D_8010FE3C (*(char (*)[])&D_8010FE3C)
#define D_801100F8 (*(int (*))&D_801100F8)

int func_80089344(short a0, short a1, short a2, short a3, int a4) {
    short t0;
    int a;
    unsigned short s;
    int e;
    unsigned short idx;
    t0 = a4 - 0x40;
    a = a0 * 0x36;
    if (*(short *)(D_8010F918 + a) == a1) {
        if (*(short *)(D_8010F920 + a) == a2) {
            if (*(short *)(D_8010F91C + a) == a3) {
                int d1, d2, d3, d4, d5, d6, d7, d8, d9;
                short ret;
                int v0 = D_80110107;
                /* ASM_BARRIER: force load order of D_80110107, D_8010F91E, and D_8010F916 */
                __asm__ volatile("" : "+r"(v0));
                idx = *(unsigned short *)(D_8010F91E + a);
                /* ASM_BARRIER: force load of D_8010F916 before shift/add into idx */
                __asm__ volatile("" : "+r"(idx));
                s = *(unsigned short *)(D_8010F916 + a);
                /* ASM_BARRIER: keep s load scheduled before shift/add */
                __asm__ volatile("" : "+r"(s), "+r"(v0), "+r"(idx));
                idx += v0 * 16;
                if (t0 > 0) {
                    int x1 = t0 * *(unsigned char *)((idx << 5) + D_801100F8 + 0xD);
                    int sign;
                    int q;
                    /* ASM_BARRIER: reserve $t0 across division so mfhi conflicts with $t0 */
                    register int r8 __asm__("$8");
                    /* ASM_BARRIER: start $t0 live range before sign shift */
                    __asm__ volatile("" : "=r"(r8));
                    sign = x1 >> 31;
                    q = x1 / 63;
                    /* ASM_BARRIER: keep sign and $t0 live across division so sra gets $v1 and mfhi gets $t2 */
                    __asm__ volatile("" : : "r"(sign), "r"(r8));
                    s += q;
                    e = (x1 - q * 63) * 2;
                } else if (t0 < 0) {
                    int x2 = t0 * *(unsigned char *)((idx << 5) + D_801100F8 + 0xC);
                    s = s + (x2 / 64) - 1;
                    e = ((x2 - ((x2 / 64) << 6)) * 2) + 0x7F;
                } else {
                    e = 0;
                }
                *(unsigned char *)D_8011010C = *(unsigned char *)(D_8010F91E + a0 * 0x36);
                *(short *)(D_8011010C + 0xC) = a0;
                ret = func_80088608(s, e & 0xFFFF);
                *(short *)(D_8010FE3C + a0 * 16) = ret;
                /* ASM_BARRIER: prevent scheduling D_8010FE18 load before D_8010FE3C store */
                __asm__ volatile("");
                *(unsigned char *)(D_8010FE18 + a0) |= 4;
                /* ASM_BARRIER: keep ret ($v0) live across D_8010FE18 update and allocate d9 to $t2 so mfhi picks $t2 */
                __asm__ volatile("" : "=r"(d1), "=r"(d2), "=r"(d3), "=r"(d4), "=r"(d5), "=r"(d6), "=r"(d7), "=r"(d8), "=r"(d9) : "r"(ret));
                /* ASM_BARRIER: end live range of d1..d9 */
                __asm__ volatile("" : : "r"(d1), "r"(d2), "r"(d3), "r"(d4), "r"(d5), "r"(d6), "r"(d7), "r"(d8), "r"(d9));
                return 1;
            }
        }
    }
    return 0;
}

#undef D_8010FE18
#undef D_8010FE3C
#undef D_801100F8
