#include "rr/overlay661_init.h"
typedef struct { char p[0xF8]; int f0, f4; unsigned short a[6]; unsigned short b[6]; short c, d; char pad[0x320 - 0x11C]; } T;

typedef struct {
    short x, y;
} CurvePt;

typedef struct {
    int a[2];
    int b[2];
    int pad10[2];
    int f18;
    int pad1C[2];
    int f24;
} RaceDist;

typedef struct {
    char pad00[0x44];
    short boostTimer44;
    char pad46[0x58 - 0x46];
    int f58;
    char pad5C[0x11C - 0x5C];
    short divisor11C;
} VehicleCtrl;

typedef struct {
    char pad00[0x1D8];
    short input1D8;
    char pad1DA[0x1E4 - 0x1DA];
    short flags1E4;
    char pad1E6[0x1F4 - 0x1E6];
    VehicleCtrl ctrl;
} Vehicle;

typedef struct {
    char pad[0x1A08];
    int f1A08;
    int f1A0C[7];
    int f1A28;
    int f1A2C;
    int f1A30;
} GlobalF4DB8;

extern T D_8010738C[];
extern CurvePt D_8009FE70[];
extern short D_8009FE78;
extern RaceDist *D_800AC738;
extern int D_800AD504;
extern short D_800F4D44;
extern GlobalF4DB8 D_800F4DB8;
extern int D_800FF6B0;
extern void func_8004F2D8();

void func_80049430(int x, int y, int z);
int func_80049558(Vehicle *a0, int a1);

void func_80049430(int x, int y, int z) {
    int i;
    T *tx = &D_8010738C[x];
    T *ty = &D_8010738C[y];
    T *tz = &D_8010738C[z];
    tx->f0 = (ty->f0 + tz->f0) / 2;
    for (i = 0; i < 6; i++) {
        tx->a[i] = (ty->a[i] + tz->a[i]) >> 1;
    }
    for (i = 0; i < 6; i++) {
        tx->b[i] = (ty->b[i] + tz->b[i]) >> 1;
    }
    tx->c = (ty->c + tz->c) / 2;
    tx->d = (ty->d + tz->d) / 2;
}

int func_80049558(Vehicle *a0, int a1) {
    int s1 = 0;
    RaceDist *s2 = D_800AC738;
    VehicleCtrl *t0 = &a0->ctrl;
    int d;
    int i;
    int lo;
    int b;
    int v2;
    int v1;
    int v0;

    if (s2->b[a1] >= D_8009FE70[0].x) {
        for (i = 1; i < 7; i++) {
            if (s2->b[a1] < D_8009FE70[i].x) {
                break;
            }
        }
        if (i < 4) {
            d = s2->b[a1] - D_8009FE70[i].x;
            lo = ((D_8009FE70[i].y - D_8009FE70[i - 1].y) * d) / (D_8009FE70[i].x - D_8009FE70[i - 1].x);
            s1 = D_8009FE70[i].y + lo;
            d = t0->f58 - s2->a[a1];
            if (d < 0) {
                d = 0;
            }
            if (d > a0->input1D8) {
                d = a0->input1D8;
            }
            if (a0->input1D8 > 0) {
                d = (d << 10) / a0->input1D8;
            }
            if (d >= 0x100) {
                d = 0x400 - (((d - 0x100) * 4) / 3);
                s1 = (s1 * d) / 1024;
            }
            i = t0->boostTimer44;
            if (i > 0) {
                i = i << 10;
                /* ASM_BARRIER: keep sll before the lh (original schedule) and keep lo global */
                __asm__ volatile("" : "=r"(i), "=r"(lo) : "0"(i));
                v1 = t0->divisor11C;
                if (v1 < 0) {
                    v1 = -v1;
                }
                s1 = (s1 * (0x400 - (i / v1))) / 1024;
            }
        } else if (i < 7) {
            int lo2;
            int denom;
            d = s2->b[a1] - D_8009FE70[i].x;
            denom = D_8009FE70[i].x - D_8009FE70[i - 1].x;
            lo2 = ((D_8009FE70[i].y - D_8009FE70[i - 1].y) * d) / denom;
            /* ASM_BARRIER: extend denom past div so local_alloc does not combine denom with lo2 */
            __asm__ volatile("" : : "r"(denom));
            s1 = D_8009FE70[i].y + lo2;
            s1 += 0x800;
        }
        if (D_800F4D44 == 1 || D_800F4D44 == 6) {
            if (D_800AC738->f18 == 0 && D_800AC738->f24 == a0->flags1E4 && s2->b[a1] > D_8009FE78) {
                s1 = 0xC00;
            }
        }
    }
    if (D_800AD504 != 2 && D_800FF6B0 < 4) {
        if (s2->b[a1] < 0x7D0) {
            if (D_800F4DB8.f1A0C[(a0->flags1E4 & 0xF) - 1] == 0) {
                if (D_800F4DB8.f1A28 == 0) {
                    v0 = func_80096464() & 1;
                    D_800F4DB8.f1A2C = v0;
                    func_8004F2D8(v0 + 0xD, 0);
                }
                D_800F4DB8.f1A0C[(a0->flags1E4 & 0xF) - 1] = 1;
                D_800F4DB8.f1A28 = 0x4B;
            }
        }
        if (s2->b[a1] >= 0x1771) {
            if (D_800F4DB8.f1A0C[(a0->flags1E4 & 0xF) - 1] != 0) {
                D_800F4DB8.f1A0C[(a0->flags1E4 & 0xF) - 1] = 0;
            }
        }
        if (D_800AC738->f18 == a0->flags1E4 && D_800AC738->f24 == 0) {
            if (s2->b[a1] >= -0x5DB) {
                if (D_800F4DB8.f1A08 == 0) {
                    if (D_800F4DB8.f1A28 == 0) {
                        v0 = func_80096464() & 1;
                        D_800F4DB8.f1A30 = v0;
                        func_8004F2D8(v0 + 0xB, 0);
                    }
                    D_800F4DB8.f1A08 = 1;
                    D_800F4DB8.f1A28 = 0x4B;
                }
            }
            if (s2->b[a1] < -0x1388) {
                if (D_800F4DB8.f1A08 != 0) {
                    D_800F4DB8.f1A08 = 0;
                }
            }
        }
    }
    return s1;
}
