#include "types.h"
#include "rr_asm.h"
typedef struct { char pad[0x20]; s32 p[3]; char pad2[0x320 - 0x2C]; } w3t1_E;
typedef struct { s16 vx, vy, vz, pad; } w3t1_SV;
typedef struct { s32 d[3]; s32 pad0; s32 m[8]; w3t1_SV sv; s32 pad1[2]; w3t1_SV rot; s32 fl; } w3t1_F;

extern char D_800F4DB8[];
extern w3t1_E D_800ABEF8[];
extern w3t1_E D_80107198[];
extern s32 D_800AC66C;
extern s32 func_80090758();
extern s32 func_80091844();
extern s32 func_800924D4();
extern s32 func_800904F4();
extern s32 func_80090424();

#define REC(b, i, o) (*(s32 *)((b) + (i) * 28 + (o)))
#define RECH(b, i, o) (*(s16 *)((b) + (i) * 28 + (o)))

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
