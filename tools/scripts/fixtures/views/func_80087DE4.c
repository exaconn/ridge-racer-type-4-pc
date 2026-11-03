/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "types.h"

typedef struct {
    unsigned char *c0;
    int c4, c8, cC;
    char p10[4];
    unsigned char b14;
    char p15[2];
    unsigned char b17;
    char p18[0x60 - 0x18];
    short h60[0x10];
    char p80[0xB0 - 0x80];
} Ch;

typedef struct {
    unsigned char f00, f01, f02, f03, f04, f05;
    char p06[0x10 - 6];
} Prog;

typedef struct {
    char p00[0x12];
    unsigned short h12;
} VabHdr;

typedef struct {
    unsigned char f00, f01, f02, f03, f04, f05;
    char p06[0x20 - 6];
} Tone;

typedef struct {
    signed char f00, f01, f02, f03, f04, f05, f06, f07, f08, f09, f0A, f0B, f0C, f0D, f0E, f0F, f10, f11, f12, f13;
    short f14, f16, f18;
} SvmCur;

typedef struct {
    short f00, f02, f04, f06, f08;
    signed char f0A, f0B;
    short f0C, f0E, f10, f12, f14, f16, f18, f1A;
    signed char f1C, f1D;
    char p1E[0x36 - 0x1E];
} SvmVoice;

extern Ch *D_8010F068[];

extern Prog *D_801100EC;

extern VabHdr *D_801100F4;

extern Tone *D_801100F8;

extern signed char D_801100FC;

extern SvmCur D_80110100;

extern SvmVoice D_8010F908[];

extern int func_8008A564();

extern int func_80088340();

extern unsigned char func_80089ED4();

extern unsigned char func_80086BE4();

extern void func_80086E74();

extern void func_80088794();

extern unsigned short func_800885D4();

extern void func_80088E74();

int func_80087DE4(short a0, short a1, short a2, short a3, int a4_in, int a5_in)
{
    unsigned char sp10[0x80];
    unsigned char sp90[0x80];
    unsigned char n;
    unsigned char i;
    short v;
    int ret;
    Ch *s5;
    Prog *prog;
    Tone *tone;
    Ch **tbl;
    int a4 = (unsigned short)a4_in;
    int a5 = (unsigned short)a5_in;

    ret = 0;
    tbl = D_8010F068;
    s5 = tbl[a0 & 0xFF] + ((a0 & 0xFF00) >> 8);
    if (func_8008A564(a1, a2) != 0) {
        return -1;
    }
    D_80110100.f14 = a0;
    D_80110100.f02 = a3;
    D_80110100.f03 = 0;
    if (a0 == 0x21) {
        D_80110100.f04 = a4;
    } else {
        /* ASM_BARRIER: pins the product to $v0; with the identical arms (merged by
           post-reload cross-jumping) this keeps mult and div in separate flow blocks,
           so the highpart stays in HI and is reloaded via $t0 like the original */
        register int t asm("$2") = a4 * ((Ch *)((s5->b17 << 1) + (int)s5))->h60[0];
        if (t < 0) {
            D_80110100.f04 = t / 127;
        } else {
            D_80110100.f04 = t / 127;
        }
    }
    D_80110100.f05 = a5;
    prog = (Prog *)(((int)a2 << 4) + (int)D_801100EC);
    D_80110100.f0A = prog->f01;
    D_80110100.f0B = prog->f04;
    D_80110100.f00 = prog->f00;
    if (D_80110100.f07 >= D_801100F4->h12) {
        return -1;
    }
    if (a4 == 0) {
        ret = func_80088340(a0, a1, a2, (unsigned short)a3);
    } else {
        n = func_80089ED4(sp90, sp10);
        for (i = 0; i < n; i++) {
            D_80110100.f16 = sp10[i];
            D_80110100.f0C = sp90[i];
            tone = (Tone *)(((int)(unsigned short)((D_80110100.f07 * 16) + D_80110100.f0C) << 5) + (int)D_801100F8);
            D_80110100.f0F = tone->f00;
            D_80110100.f0D = tone->f02;
            D_80110100.f0E = tone->f03;
            D_80110100.f10 = tone->f04;
            D_80110100.f11 = tone->f05;
            D_80110100.f12 = tone->f01;
            v = func_80086BE4(0);
            D_80110100.f18 = v;
            if (v < D_801100FC) {
                int a0_ext = a0;
                D_8010F908[D_80110100.f18].f1D = 1;
                D_8010F908[D_80110100.f18].f02 = 0;
                D_8010F908[D_80110100.f18].f10 = a0;
                D_8010F908[D_80110100.f18].f18 = D_80110100.f01;
                D_8010F908[D_80110100.f18].f12 = D_80110100.f07;
                D_8010F908[D_80110100.f18].f14 = a2;
                if (a0_ext != 0x21) {
                    D_8010F908[D_80110100.f18].f08 = a4;
                    D_8010F908[D_80110100.f18].f0C = s5->b17;
                }
                D_8010F908[D_80110100.f18].f0A = a5;
                D_8010F908[D_80110100.f18].f16 = D_80110100.f0C;
                D_8010F908[D_80110100.f18].f0E = a3;
                D_8010F908[D_80110100.f18].f1A = D_80110100.f0F;
                D_8010F908[D_80110100.f18].f00 = D_80110100.f16;
                func_80086E74();
                if (D_80110100.f16 == 0xFF) {
                    func_80088794((unsigned char)D_80110100.f18);
                } else {
                    func_80088E74(n, func_800885D4());
                }
                ret |= 1 << D_80110100.f18;
            } else {
                ret = -1;
            }
        }
    }
    return ret;
}
