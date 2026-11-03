#include "types.h"
#include "rr_asm.h"
#include "game/sound/sndown.h"
#include "game/sound/sndcmd.h"
#include "rr/bios_devices.h"
#include "rr/overlay662_distance.h"

#define REC(b, i, o) (*(s32 *)((b) + (i) * 28 + (o)))
#define RECH(b, i, o) (*(s16 *)((b) + (i) * 28 + (o)))

typedef struct {
    s32 unk00;
    s16 unk04;
    s16 unk06;
    s32 unk08[5];
} Rec1C;

typedef struct {
    char pad0[0x18C4];
    s32 pos18C4[3];
    char pad1[0x18E4 - 0x18D0];
    s32 unk18E4;
    s32 unk18E8;
    Rec1C rec[2];
} State800F4DB8;

typedef struct {
    char pad0[0x94];
    s16 unk94;
    char pad1[0xC0 - 0x96];
    s32 unkC0;
    s32 unkC4;
    char pad2[0x12C - 0xC8];
} Control12C;

typedef struct {
    char pad0[0x20];
    s32 pos20[3];
    char pad1[0x184 - 0x2C];
    s32 first184;
    s32 second188;
    char pad2[0x1F4 - 0x18C];
    Control12C sub1F4;
} Vehicle320;

typedef struct { char pad[0x20]; s32 p[3]; char pad2[0x320 - 0x2C]; } w3t1_E;

typedef struct { s16 vx, vy, vz, pad; } w3t1_SV;

typedef struct { s32 d[3]; s32 pad0; s32 m[8]; w3t1_SV sv; s32 pad1[2]; w3t1_SV rot; s32 fl; } w3t1_F;

extern struct {
    char pad0[0x18C0]; short a; short b; char pad1[0x28];
    int c; char pad2[0x18]; int d; char pad3[0x18]; int e; short f; char pad4[0x16]; int g; short h;
} D_800F4DB8;
extern int D_800F66A0;
extern s32 D_800F2EB8;
extern struct {
    char pad0[0x10];
    s32 count10;
} D_800AC650;
extern s32 D_800AC660;
extern Vehicle320 D_80107198[];
extern s32 D_800F6748;
extern w3t1_E D_800ABEF8[];
extern s32 func_80091844();
extern s32 func_800924D4();
extern s32 func_800904F4();
extern s32 func_80090424();

void func_80051B44(short arg0, short arg1);
void func_80051BD4(s32 arg0);
void func_80052024(s32 idx);
void func_800523C8(int arg0);

void func_80051B44(short arg0, short arg1) {
    int t;
    D_800F4DB8.a = arg0;
    D_800F4DB8.b = arg1;
    func_80096434((char *)&D_800F4DB8 + 0x18EC, 0x70);
    t = arg1 & 0xFFFF;
    switch (t) {
    case 0:
        D_800F4DB8.c = 1;
        D_800F4DB8.d = 1;
        D_800F4DB8.e = 0;
        D_800F4DB8.f = 0;
        break;
    case 1:
        D_800F4DB8.e = 0;
        D_800F4DB8.g = 0;
        D_800F4DB8.f = 0;
        D_800F4DB8.h = t;
        break;
    }
    D_800F66A0 = 4;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(State800F4DB8 (*))&D_800F4DB8)

void func_80051BD4(s32 arg0) {
    s32 d[4];
    s32 min_dist[2];
    s32 min_idx[2];
    s32 prev_idx[2];
    s32 slot[2];
    s32 i;
    s32 dist;
    /* ASM_BARRIER: allocate car pointer to v1 to match original register allocation */
    register Vehicle320 *car asm("$3");
    Control12C *sub;

    prev_idx[0] = D_800F4DB8.rec[0].unk04;
    prev_idx[1] = D_800F4DB8.rec[1].unk04;

    switch (arg0) {
    case 0:
        min_idx[1] = -1;
        min_idx[0] = -1;
        min_dist[1] = D_800F2EB8;
        min_dist[0] = D_800F2EB8;
        for (i = 0; i < D_800AC660; i++) {
            car = &D_80107198[i];
            dist = (car->first184 + car->second188) - D_800F4DB8.unk18E4;
            dist %= D_800F2EB8;
            if (dist >= D_800F2EB8 / 2) {
                dist -= D_800F2EB8;
            }
            if (dist < -D_800F2EB8 / 2) {
                dist += D_800F2EB8;
            }
            if (dist < 0) {
                dist = -dist;
            }
            if (dist < min_dist[0]) {
                min_dist[1] = min_dist[0];
                min_idx[1] = min_idx[0];
                min_dist[0] = dist;
                min_idx[0] = i;
            } else if (dist < min_dist[1]) {
                min_dist[1] = dist;
                min_idx[1] = i;
            }
        }
        goto update_slots;
    case 1:
        min_dist[1] = 0x7FFFFFFF;
        min_dist[0] = 0x7FFFFFFF;
        min_idx[1] = -1;
        min_idx[0] = -1;
        for (i = 0; i < D_800AC650.count10; i++) {
            d[0] = (D_80107198[i].pos20[0] - D_800F4DB8.pos18C4[0]) / 16;
            d[1] = (D_80107198[i].pos20[1] - D_800F4DB8.pos18C4[1]) / 16;
            d[2] = (D_80107198[i].pos20[2] - D_800F4DB8.pos18C4[2]) / 16;
            dist = func_80090758(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
            if (dist < min_dist[0]) {
                min_dist[1] = min_dist[0];
                min_idx[1] = min_idx[0];
                min_dist[0] = dist;
                min_idx[0] = i;
            } else if (dist < min_dist[1]) {
                min_dist[1] = dist;
                min_idx[1] = i;
            }
        }
    update_slots:
        slot[0] = 0;
        slot[1] = 1;
        if (min_idx[0] == D_800F4DB8.rec[1].unk04) {
            slot[0] = 1;
            slot[1] = 0;
        }
        if (min_idx[1] == D_800F4DB8.rec[0].unk04) {
            slot[0] = 1;
            slot[1] = 0;
        }
        D_800F4DB8.rec[slot[0]].unk04 = min_idx[0];
        D_800F4DB8.rec[slot[1]].unk04 = min_idx[1];
        break;
    }

    for (i = 0; i < 2; i++) {
        D_800F4DB8.rec[i].unk06 = 0;
        if (prev_idx[i] != D_800F4DB8.rec[i].unk04) {
            D_800F4DB8.rec[i].unk06 = 1;
        }
    }

    sub = &D_80107198[min_idx[0]].sub1F4;
    if (sub->unk94 != 0) {
        func_80050E94(1, sub->unkC0, sub->unkC4, 1);
        D_800F6748 = slot[0] + 2;
    } else {
        sub = &D_80107198[min_idx[1]].sub1F4;
        if (sub->unk94 != 0) {
            func_80050E94(1, sub->unkC0, sub->unkC4, 1);
            D_800F6748 = slot[1] + 2;
        } else {
            func_80050E94(0, 0, 0, 1);
        }
    }
}

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(char (*)[])&D_800F4DB8)
#define D_80107198 (*(w3t1_E (*)[])&D_80107198)
#define func_80090758 ((s32 (*)())func_80090758)

void func_80052024(s32 idx) {
    w3t1_F f;
    char *B = D_800F4DB8;
    char *B2, *B3;
    w3t1_E *e;
    s32 rx, ry, rz;
    s32 mode, a, c, t, ang, prev, t2;
    s32 s2;
    s32 alpha;
    REC(B, idx, 0x18F8) = REC(B, idx, 0x18F4);
    mode = REC(B, idx, 0x18EC);
    switch (mode) {
    case 0:
        e = &D_800ABEF8[RECH(B, idx, 0x18F0)];
        f.d[0] = (e->p[0] - *(s32 *)(B + 0x18C4)) / 16;
        f.d[1] = (e->p[1] - *(s32 *)(B + 0x18C8)) / 16;
        f.d[2] = (e->p[2] - *(s32 *)(B + 0x18CC)) / 16;
        break;
    case 1:
        e = &D_80107198[RECH(B, idx, 0x18F0)];
        f.d[0] = (e->p[0] - *(s32 *)(B + 0x18C4)) / 16;
        f.d[1] = (e->p[1] - *(s32 *)(B + 0x18C8)) / 16;
        f.d[2] = (e->p[2] - *(s32 *)(B + 0x18CC)) / 16;
        break;
    }
    t = func_80090758(f.d[0] * f.d[0] + f.d[1] * f.d[1] + f.d[2] * f.d[2]);
    B2 = D_800F4DB8;
    prev = REC(B2, idx, 0x18F8);
    REC(B2, idx, 0x18F4) = t;
    a = prev - t;
    REC(B2, idx, 0x18FC) = a;
    if (a == 0x2350) REC(B2, idx, 0x18FC) = 0x234F;
    rx = *(u16 *)(B2 + 0x18D4);
    f.m[7] = 0;
    f.m[6] = 0;
    f.m[5] = 0;
    f.rot.vx = rx;
    ry = *(u16 *)(B2 + 0x18D8);
    rz = *(u16 *)(B2 + 0x18DC);
    f.rot.vy = -ry;
    f.rot.vz = rz;
    func_80091844(&f.rot, f.m);
    RR_GTE_SETROT_TR(f.m);
    f.sv.vx = f.d[0];
    f.sv.vy = f.d[1];
    f.sv.vz = f.d[2];
    RR_GTE_MVMVA_ROT_TR_V0(&f.sv);
    RR_GTE_SWC2_MAC123(f.d);
    RR_GTE_STORE_FLAG(&f.fl);
    ang = func_800924D4(-f.d[2], f.d[0]);
    s2 = func_800904F4(ang) / 64 + 0x40;
    if (s2 >= 0x80) s2 = 0x7F;
    c = func_80090424(ang) / 64 + 0x40;
    if (c >= 0x80) c = 0x7F;
    t2 = REC(B2, idx, 0x18F4);
    if (t2 > 0x2800) {
        alpha = 0;
    } else {
        alpha = 0x2800 - t2;
        alpha = (alpha * alpha) / 737280;
        if (alpha > 0x80) alpha = 0x80;
    }
    if (D_800AC66C) alpha = 0;
    B3 = D_800F4DB8;
    t2 = REC(B3, idx, 0x18FC);
    if (t2 < 0) t2 = -t2;
    /* ASM_BARRIER: launder abs result so reorg cannot copy the slti into the bgez delay slot */
    __asm__("" : "=r"(t2) : "0"(t2));
    if (t2 > 0x800) alpha = 0;
    if (*(s32 *)(B3 + 0x18E8)) alpha = 0;
    RECH(B3, idx, 0x1902) = alpha;
    RECH(B3, idx, 0x1900) = s2;
    REC(B3, idx, 0x1904) = c;
}

#undef D_800F4DB8
#undef D_80107198
#undef func_80090758

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(char (*)[])&D_800F4DB8)

void func_800523C8(int arg0) {
 int k = arg0 * 3; int t = arg0 + 2; char *v, *s, *b;
 func_80052024(t); b = D_800F4DB8; s = b + (((k << 4) + arg0) << 5); v = b + t * 0x1C;
 *(int *)(s + 0x68) = *(int *)(v + 0x18FC);
 *(unsigned short *)(s + 0x5C) = *(unsigned short *)(v + 0x1900);
 *(unsigned short *)(s + 0x5A) = *(unsigned short *)(v + 0x1902);
 *(int *)(s + 0x64) = *(int *)(v + 0x1904);
}

#undef D_800F4DB8
