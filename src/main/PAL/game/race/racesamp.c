/* RODATA 0x80010700 0x1c */
#include "types.h"
#include "rr/track_sample_types.h"
#include "m2c_macros.h"
#include "game/car/carmove1.h"
#include "game/menu/menu8.h"
#include "game/course/trkintp.h"
#include "game/ui/uipart1.h"
#include "game/car/carparam.h"
#include "game/car/carsetup.h"
#include "rr/audio_loop.h"

typedef struct { char x[0x320]; } S;

typedef struct {
    int a, b, c, d;
} Vec4;

typedef struct {
    int f0;
    int pad[3];
    struct {
        int x, z;
    } pos[8];
} Grid53C;

typedef struct {
    unsigned short w0;
    short w2;
    unsigned short w4;
    short w6;
} Bounds08;

typedef struct {
    int f0;
    int f4;
    int f8;
    char pad0C[0x20 - 0x0C];
    int f20;
    int f24;
    int f28;
    int f2C;
    char pad30[0x50 - 0x30];
    int f50;
    int f54;
    int f58;
    char pad5C[0x70 - 0x5C];
    Vec4 vec70;
    short s80;
    short s82;
    short s84;
    char pad86[2];
    struct {
        int f0;
        int f4;
        int f8;
        int fC;
    } sub88[4];
    char padC8[0xDC - 0xC8];
    short sDC;
    short sDE;
    short sE0;
    short sE2;
    short sE4;
    short sE6;
    short sE8;
    short sEA;
    char padEC[4];
    int fF0;
    int fF4;
    short sF8;
    short sFA;
    short sFC;
    short sFE;
    char pad100[8];
    int f108;
    short s10C;
    short s10E;
    short s110;
    short s112;
    short s114;
    short s116;
    short s118;
    short s11A;
    short s11C;
    short s11E;
    short s120;
    short s122;
    short arr124[16];
    short arr144[16];
    unsigned short s164;
    unsigned short s166;
    unsigned short s168;
    unsigned short s16A;
    int f16C;
    char pad170[0x18C - 0x170];
    int f18C;
    int f190;
    char pad194[0x1A8 - 0x194];
    short s1A8;
    short s1AA;
    short s1AC;
    short s1AE;
    short s1B0;
    short s1B2;
    short s1B4;
    short s1B6;
    char pad1B8[4];
    short arr1BC[4];
    short arr1C4[4];
    char pad1CC[0x1D4 - 0x1CC];
    short s1D4;
    short s1D6;
    char pad1D8[0x1E4 - 0x1D8];
    short s1E4;
    short s1E6;
    char pad1E8[0x290 - 0x1E8];
    int f290;
} Car37618;

extern int func_80014A8C();
extern int func_8003D228();
extern int func_8003DC9C();
extern int func_8003FAE4();
extern s32 D_800F2EC8;
extern int func_8001FCC8();
extern int func_8001FF84();
extern int D_800ACC14;
extern unsigned char D_8009F7CC[];
extern unsigned char D_8009F7D0[];
extern unsigned char D_8009F7D4[];
extern unsigned char D_8009F7D8[];
extern unsigned char D_8009F7DC[];
extern struct { char pad[0x1D2]; short h1D2; char pad2[0x1F]; unsigned char b1F3; } *D_800F39C8[];
extern unsigned char D_800F4D98[][4];
extern S *D_800FFC20[];
extern S D_800ABEF8[];
extern S D_80106B58[];
extern S D_80106E78[];
extern S D_80107198[];
extern short D_800AC59C;
extern short D_800AD508;
extern int func_80026A74();
extern int func_8006E368();
extern int func_80026F5C();
extern int func_80027C54();
extern short D_800AD522;
extern struct {
    char pad[0x53C];
    Grid53C *p53C;
} D_800AC650;
extern Bounds08 *D_800F3A08[];

void func_80037110(void);
void func_80037198(int arg0);
void func_800373DC(int a);
void func_80037618(Car37618 *p, int arg1);

void func_80037110(void) {
    func_80014A8C(0x19);
    D_800F2EC8 = 0;
    func_8003D228(0, 0x10, 0x158, 0x120, 0x3C, 0, 0x103);
    func_8003DC9C(0, 0x10, 0x48, 0x120, 0x120, 0, 0);
    func_8003FAE4(0, -1, 0, 1);
}

void func_80037198(int arg0) {
    int v[9];
    char buf[4];
    int s0;
    int a1;
    int b;
    unsigned char *t7 = D_8009F7CC;
    unsigned char (*q)[4] = D_800F4D98;
    unsigned char *p = q[arg0];
    v[0] = t7[p[1]];
    v[1] = D_8009F7D0[p[1]];
    v[2] = D_800F39C8[arg0]->h1D2 > 0 ? 0x18 : 0x19;
    v[3] = D_8009F7D4[p[0]];
    v[4] = D_8009F7D8[p[0]];
    v[5] = D_8009F7DC[p[0]];
    v[6] = D_800F39C8[arg0]->b1F3 + 0x14;
    b = p[3];
    v[7] = b + 0x1A;
    v[8] = b + 0x1C;
    if (p[2] == 0x27) {
        v[5] = 0x1E;
        v[4] = 0x1E;
        v[3] = 0x1E;
        v[1] = 0x1E;
        v[0] = 0x1E;
    }
    if (p[3] == 2) {
        v[8] = 0x1E;
        v[7] = 0x1E;
    }
    s0 = 0;
    a1 = *(int *)0x1F800000;
    buf[2] = 0x10;
    buf[3] = 0x20;
    do {
        int t = v[s0];
        buf[0] = (t % 16) * 16;
        buf[1] = (t / 16) << 5;
        a1 = func_8001FF84(D_800ACC14 + 0x70, a1, (s0 % 3) * 0x14 + 0xF0, (s0 / 3) * 0x28 + 0x58, buf, 0x7FA4, 1);
        s0++;
    } while (s0 < 9);
    *(int *)0x1F800000 = func_8001FCC8(D_800ACC14 + 0x70, a1, 0x39);
}

void func_800373DC(int a) {
    int i;
    int n;
    int v;
    S **p;
    S *q;
    switch (a) {
    case 1:
        D_800AC59C = 8;
        i = 0;
        for (; i < D_800AC59C; i++) {
            if (i == 0) {
                D_800FFC20[0] = D_800ABEF8;
            } else {
                D_800FFC20[i] = D_80106E78 + i;
            }
        }
        break;
    case 2:
        D_800AC59C = 2;
        D_800FFC20[0] = D_800ABEF8;
        D_800FFC20[1] = D_800ABEF8 + 1;
        break;
    case 3:
        i = 2;
        D_800FFC20[0] = D_800ABEF8;
        D_800FFC20[1] = D_800ABEF8 + 1;
        v = D_800AC59C;
        if (i < v) {
            n = v;
            do {
                D_800FFC20[i] = D_80106B58 + i;
                i++;
            } while (i < n);
        }
        break;
    case 4:
        D_800AC59C = 4;
        if (D_800AD508 == 1) {
            D_800FFC20[0] = D_800ABEF8;
            D_800FFC20[1] = D_800ABEF8 + 1;
            D_800FFC20[2] = D_80107198;
            D_800FFC20[3] = D_80107198 + 1;
        } else {
            D_800FFC20[2] = D_800ABEF8;
            D_800FFC20[3] = D_800ABEF8 + 1;
            D_800FFC20[0] = D_80107198;
            D_800FFC20[1] = D_80107198 + 1;
        }
        break;
    case 5:
        D_800AC59C = 7;
        i = 0;
        n = 7;
        for (; i < n; i++) {
            D_800FFC20[i] = D_80107198 + i;
        }
        break;
    default:
    case 0:
        D_800FFC20[0] = D_800ABEF8;
        D_800AC59C = 1;
        break;
    case 6:
        D_800FFC20[0] = D_800ABEF8;
        D_800FFC20[1] = D_80107198;
        D_800AC59C = 2;
        break;
    }
    func_80031DEC();
}

void func_80037618(Car37618 *p, int arg1) {
    int i;
    int t;
    int *g = (int *)0x1F80007C;

    func_8002BEF8();
    if (D_800F4D44 == 0 && D_800AD522 == 1) {
        p->fF0 = *D_800ACB8C;
        t = D_800ABECC - 5;
        p->f20 = D_800ABEA4[(p->fF0 + t) % D_800ABECC].x08;
        p->f24 = 0;
        p->f28 = D_800ABEA4[(p->fF0 + t) % D_800ABECC].z0C;
    } else {
        p->f20 = D_800AC650.p53C->pos[arg1].x;
        p->f24 = 0;
        p->f28 = D_800AC650.p53C->pos[arg1].z;
        p->fF0 = D_800AC650.p53C->f0;
    }
    p->f0 = p->f20 << 8;
    p->f4 = 0;
    p->f8 = p->f28 << 8;
    p->fF0 = func_80026A74(&p->f20, p->fF0, D_800ABECC + 1);
    p->sF8 = p->sFA = p->sFC = p->sFE = p->fF0;
    p->f50 = 0;
    p->f54 = ((0x800 - D_800ABEA4[p->fF0].heading12) - (D_800AD522 << 11)) & 0xFFF;
    p->f58 = 0;
    p->s1AE = 0;
    p->s1D4 = p->s1D6 = p->f54;
    if (p->s1E4 >= 0x10) {
        p->f290 = p->s1D4;
    }
    i = p->s1E6 >> 1;
    p->f108 = D_800F3A08[i]->w6 * 4;
    p->s10C = ((short)D_800F3A08[i]->w0 + 5) * 2;
    p->s10E = 0x28 - D_800F3A08[i]->w4;
    p->s110 = -((D_800F3A08[i]->w0 + 5) * 4);
    p->s112 = -((D_800F3A08[i]->w4 - 0x14) * 4);
    p->s114 = (D_800F3A08[i]->w0 + 5) * 4;
    p->s116 = -((D_800F3A08[i]->w4 - 0x14) * 4);
    p->s118 = -((D_800F3A08[i]->w0 + 5) * 4);
    p->s11A = -0x50;
    p->s11C = (D_800F3A08[i]->w0 + 5) * 4;
    p->s11E = -0x50;
    p->s120 = 0x400 - func_8006E368(p->s114 * 2, p->s116 + 0x50);
    p->s122 = 0x400 - func_8006E368(p->s11C * 2, p->s11E - p->s116);
    p->f16C = 0x10;
    p->sDC = p->sDE = 0;
    p->sE0 = p->sE2 = 0;
    p->sE4 = p->sE6 = p->f108 << 6;
    p->sE8 = p->sEA = p->f108 << 6;
    p->s80 = p->f50;
    p->s82 = p->f54;
    p->s84 = p->f58;
    p->vec70 = *(Vec4 *)&p->f20;
    p->s1AC = p->s1A8 = p->s1B4 = p->s1B0 = 0;
    p->s1AE = p->s1AA = p->s1B6 = p->s1B2 = 0;
    p->s164 = p->s166 = 0;
    for (i = 0; i < 10; i++) {
        p->s164 += p->f50;
        p->s166 += p->f58;
    }
    p->s168 = p->s16A = 0;
    for (i = 0; i < 10; i++) {
        p->s168 += p->f50;
        p->s16A += p->f58;
    }
    for (i = 0; i < 16; i++) {
        p->arr124[i] = p->f50;
        p->arr144[i] = p->f58;
    }
    for (i = 0; i < 4; i++) {
        p->sub88[i].f4 = p->f24;
        p->arr1BC[i] = 0;
        p->arr1C4[i] = 0;
    }
    func_80026004(p, 0);
    p->f190 = p->f18C;
    func_800244B0(p);
    func_80022888(p);
    func_80026F5C(p);
    *g = 0;
    func_80027C54(p);
    func_80028B30(p);
}
