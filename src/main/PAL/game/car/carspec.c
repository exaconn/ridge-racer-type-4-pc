#include "rr/race.h"

#define D_800AC650 D_800AC650_prefix
#undef D_800AC650

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

extern char D_8010738C[];
extern RrRaceSpecEntry30 *D_800ACB90;
extern CurvePt D_8009FE70[];
extern short D_8009FE78;
extern RaceDist *D_800AC738;
extern int D_800AD504;
extern short D_800F4D44;
extern GlobalF4DB8 D_800F4DB8;
extern int D_800FF6B0;
extern int func_80096464();
extern void func_8004F2D8();

void func_80049228(int a0, int a1, int a2, int a3);
void func_80049430(int x, int y, int z);
int func_80049558(Vehicle *a0, int a1);
void func_80049A60();

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(struct { char pad[0x544]; RrRaceSpecEntry30 *p[1]; } (*))&D_800AC650)

void func_80049228(int a0, int a1, int a2, int a3) {
    RrRaceSpecEntry30 *t3 = &D_800ACB90[a0];
    RrRaceVehicleControl *t2 = (RrRaceVehicleControl *)(D_8010738C + a1 * sizeof(RrRaceVehicle));
    int lo, v, i, a;
    if (a2 < 0x401) {
        lo = a2 * t2->factor11E;
        v = lo >> 10;
        if (lo < 0) v = (lo + 0x3FF) >> 10;
    } else {
        v = a2 - 0x800;
    }
    a = t3->baseSpeed00;
    t2->maxSpeedF8 = ((((D_800AC650.p[a3]->baseSpeed00 - a) * v) / 1024) + a) << 8;
    i = 0;
    do {
        t2->gearRatios100[i] = ((((D_800AC650.p[a3]->ratios08[i] - t3->ratios08[i]) * v) / 1024) + t3->ratios08[i]) * 4;
        i++;
    } while (i < 6);
    i = 0;
    do {
        t2->shiftThresholds10C[i] = t3->thresholds14[i] + (((D_800AC650.p[a3]->thresholds14[i] - t3->thresholds14[i]) * v) / 1024);
        i++;
    } while (i < 6);
    t2->maximum118 = (unsigned short)t3->maximum20 + (((D_800AC650.p[a3]->maximum20 - t3->maximum20) * v) / 1024);
    t2->span11A = (unsigned short)t3->span22 + (((D_800AC650.p[a3]->span22 - t3->span22) * v) / 1024);
}

#undef D_800AC650

/* legacy typing of shared globals in this part of the unit */
#define D_8010738C (*(T (*)[])&D_8010738C)

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

#undef D_8010738C

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(struct { char pad[0x14]; int mode; char pad2[0x528]; RrRaceSpecEntry30 *tab; } (*))&D_800AC650)

void func_80049A60(a0) RrRaceVehicle *a0; {
    int s0 = 0;
    RrRaceSpecEntry30 *s2;
    RrRaceVehicleControl *s3;
    int a, b;
    int lo;
    /* ASM_BARRIER: pin loop counter/pointers/temp to original a3/a2/a1/a0 */
    register int k asm("$7");
    register short * a2 asm("$6");
    register short * a1 asm("$5");
    register int tt asm("$4");
    int w;
    int v1;
    register int v2;
    s2 = D_800AC650.tab + (a0->flags1E4 & 0xF) - 1;
    s3 = (RrRaceVehicleControl *)((char *)a0 + 0x1F4);
    if (D_800AC650.mode == 1) {
        s0 = func_80049558(a0, s0);
        a = (a0->flags1E4 & 0xF) - 1;
        func_80049228(a, a, s0, 0);
    } else if (D_800AC650.mode == 2) {
        func_80049228(0, 2, func_80049558(a0, 0), 0);
        s0 = func_80049558(a0, 1);
        func_80049228(1, 3, s0, 0);
        func_80049430((a0->flags1E4 & 0xF) - 1, 2, 3);
    }
    b = s3->boostTimer44;
    if (b > 0) {
        if (D_800F4D44 == 1) {
            v2 = b << 10;
            /* ASM_BARRIER: keep sll before the lh (original schedule) */
            __asm__ volatile ("" : "=r"(v2) : "0"(v2));
            v1 = s3->divisor11C;
            if (v1 < 0) v1 = -v1;
            lo = v2 / v1;
            k = 1;
            if (s0 < lo) s0 = lo;
            a2 = (short *)((char *)s2 + 2);
            a1 = (short *)((char *)s3 + 2);
            do {
                v1 = s2->ratios08[0];
                tt = ((RrRaceSpecEntry30 *)a2)->ratios08[0];
                ((RrRaceVehicleControl *)a1)->gearRatios100[0] = ((((v1 - tt) * s0) / 1024) + tt) * 4;
                a2++;
                k++;
                a1++;
            } while (k < 6);
            v1 = s2->altSpeed28;
            w = s2->baseSpeed00;
            s3->maxSpeedF8 = ((((v1 - w) * lo) / 1024) + w) << 8;
        }
    }
}

#undef D_800AC650
