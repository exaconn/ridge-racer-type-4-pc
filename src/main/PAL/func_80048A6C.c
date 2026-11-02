#include "types.h"
#include "rr_asm.h"
typedef struct { s16 vx, vy, vz, pad; } w3t1_SV;
typedef struct { s32 m[8]; w3t1_SV sv; w3t1_SV out; w3t1_SV rot; } w3t1_F;

extern s32 func_80091844();
extern s32 func_800468F0();

#define I(p, o) (*(s32 *)((char *)(p) + (o)))
#define H(p, o) (*(s16 *)((char *)(p) + (o)))
#define U(p, o) (*(u16 *)((char *)(p) + (o)))

void func_80048A6C(char *a0) {
    w3t1_F f;
    char *s1 = a0;
    char *s0 = s1 + 0x1F4;
    s32 v0, v1, a1;
    register s32 a __asm__("$4"); /* ASM_BARRIER: pin a to $4 to reproduce a0/a1 register split of the original */
    a1 = I(s0, 0x50);
    if (a1 != 0) {
        v1 = H(s0, 0x44);
        if (v1 > 0) {
            a = H(s0, 0x11C);
            v0 = a;
            if (a < 0) v0 = -v0;
            v1 = v0 - v1;
            v1 = (a1 * v1) / v0;
            if (a > 0) {
                v0 = H(s0, 0x11E);
                v0 = (v1 * v0) / 1024;
                v1 = I(s0, 0x4C);
                v1 += a1 + v0;
                I(s0, 0x4C) = v1;
            } else {
                I(s0, 0x4C) = I(s0, 0x4C) + v1;
            }
            U(s0, 0x44) = U(s0, 0x44) - 1;
        } else {
            I(s0, 0x4C) = I(s0, 0x4C) + a1;
        }
        H(s1, 0x23A) = 0;
        v0 = I(s0, 0x4C);
        v1 = I(s0, 0xF8);
        if (v1 < v0) I(s0, 0x4C) = v1;
    } else {
        if (H(s0, 0x48) == 0) {
            if (H(s0, 0x46) != 0) {
                I(s0, 0x4C) = (I(s0, 0x4C) * 94) / 100;
            } else {
                I(s0, 0x4C) = (I(s0, 0x4C) * 99) / 100;
            }
        }
    }
    U(s1, 0x1DA) = U(s1, 0x1D8);
    H(s1, 0x1D8) = I(s0, 0x4C) / 256;
    f.rot.vx = 0;
    f.rot.vy = U(s1, 0x1D4);
    f.rot.vz = 0;
    func_80091844(&f.rot, f.m);
    f.sv.vx = 0;
    f.sv.vy = 0;
    f.sv.vz = H(s1, 0x1D8) / 16;
    RR_GTE_SETROT(f.m);
    RR_GTE_MVMVA_ROT_V0_NOCV(&f.sv);
    RR_GTE_STORE_IR_S16(&f.out);
    v0 = I(s0, 0xFC);
    v1 = H(s0, 0x68);
    H(s0, 0x62) = 0;
    if (v1 < v0 - 1) {
        if (H(s1, 0x1D8) > (s32)*(u16 *)((char *)s0 + (v1 << 1) + 0x10C)) {
            H(s0, 0x62) = 1;
            H(s0, 0x64) = 1;
        }
    }
    v0 = H(s0, 0x68);
    if (v0 > 0) {
        v0 = v0 - 1;
        v1 = *(u16 *)((char *)s0 + (v0 << 1) + 0x10C) - 0x24;
        if (v1 < 0) v1 = 0;
        if (H(s1, 0x1D8) < v1) {
            v1 = U(s0, 0x56);
            H(s0, 0x62) = -1;
            H(s0, 0x64) = -1;
            v0 = (0x2800 - v1) / 4;
            H(s0, 0x66) = v0;
            if ((s16)v0 < 0) H(s0, 0x66) = 0;
        }
    }
    I(s1, 0x30) = f.out.vx;
    I(s1, 0x38) = f.out.vz;
    func_800468F0(s1);
}
