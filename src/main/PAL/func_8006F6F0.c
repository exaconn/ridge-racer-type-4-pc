#include "types.h"
#include "rr_asm.h"

typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;

typedef struct {
    s32 vx, vy, vz, pad;
} VECTOR;

typedef struct {
    s16 vx, vy, vz, pad;
} SVECTOR;

typedef struct {
    s32 pad00[2];
    s32 x08;
    s32 y0C;
    s32 z10;
    s32 pad14[20];
    s32 flag64;
    s32 pad68[4];
    s32 col78;
} Scratch;

#define G ((Scratch *)0x1F800000)

extern void func_8005F0D0(s32, s32, s32, s32);
extern void func_8006E030(MATRIX *, s32);
extern void func_8006E09C(MATRIX *, s32);
extern void func_8006E108(MATRIX *, s32);
extern void func_8006EE3C(s32, void *, MATRIX *);
extern void func_800701C4(s32);
extern void func_80090914(void *, MATRIX *, MATRIX *);
extern void func_80090CA4(MATRIX *, MATRIX *);
extern void func_80090DB4(MATRIX *, MATRIX *);
extern void func_800910E4(MATRIX *);

extern s32 D_800ABED0[];
extern s32 D_800F2FD8;
extern s16 *D_800F3A08[];

#define S16(p, o) (*(s16 *)((char *)(p) + (o)))
#define S32(p, o) (*(s32 *)((char *)(p) + (o)))
#define VEC(p, o) (*(VECTOR *)((char *)(p) + (o)))

s32 func_8006F6F0(char *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    MATRIX m10;
    MATRIX m30;
    MATRIX m50;
    MATRIX m70;
    MATRIX m90;
    MATRIX mB0;
    SVECTOR svD0;
    MATRIX mD8[6];
    VECTOR v198[5];
    s32 dist;
    s32 i;
    s32 s5;
    s32 a1;

    func_800701C4(arg1);
    v198[0].vx = S16(arg0, 0x20) - (s16)G->x08;
    v198[0].vy = 0;
    v198[0].vz = S16(arg0, 0x28) - (s16)G->z10;
    if (v198[0].vx < 0) {
        v198[0].vx = -v198[0].vx;
    }
    if (v198[0].vz < 0) {
        v198[0].vz = -v198[0].vz;
    }
    dist = v198[0].vx + v198[0].vz;
    switch (arg2) {
    case 0:
        i = dist;
        break;
    case 1:
        i = arg3 - 1;
        break;
    case 2:
        i = arg4 - 1;
        break;
    case 3:
        i = arg5 - 1;
        break;
    }
    if (i < arg3) {
        s5 = 0;
        if (G->flag64 != 0) {
            s5 = -0x60;
        }
        func_8006E09C(&m90, -S32(arg0, 0x54));
        func_8006E108(&m50, -S32(arg0, 0x50));
        func_80090DB4(&m90, &m50);
        func_8006E030(&m70, -S32(arg0, 0x58));
        func_80090DB4(&m50, &m70);
        func_80090914(D_800ABED0, &m70, &mB0);
        func_800910E4(&mB0);
        func_8006E09C(&m10, -S32(arg0, 0x54));
        func_80090DB4((MATRIX *)0x1F800028, &m10);
        func_8006E108(&mD8[0], S16(arg0, 0x1A8) - S32(arg0, 0x50));
        func_8006E030(&m50, S16(arg0, 0x1AA) - S32(arg0, 0x58));
        func_80090CA4(&mD8[0], &m50);
        func_80090DB4(&m10, &mD8[0]);
        v198[0] = VEC(arg0, 0xC8);
        v198[1].vx = S32(arg0, 0x70) + S32(arg0, 0x60);
        v198[1].vy = S32(arg0, 0x74) + S32(arg0, 0x64);
        v198[1].vz = S32(arg0, 0x78) + S32(arg0, 0x68);
        func_8006E108(&mD8[1], -S16(arg0, 0x80));
        func_8006E030(&m50, -S16(arg0, 0x84));
        func_80090CA4(&mD8[1], &m50);
        func_80090DB4(&m10, &mD8[1]);
        v198[2].vx = S32(arg0, 0x10) + S32(arg0, 0x60);
        v198[2].vy = S32(arg0, 0x14) + S32(arg0, 0x64);
        v198[2].vz = S32(arg0, 0x18) + S32(arg0, 0x68);
        func_8006E108(&m30, -S32(arg0, 0x50));
        func_8006E108(&mD8[3], S32(arg0, 0x17C));
        func_8006E030(&mD8[2], -S32(arg0, 0x58));
        func_80090DB4(&m30, &mD8[2]);
        m30 = mD8[2];
        func_80090DB4(&m10, &mD8[2]);
        func_80090DB4(&mD8[2], &mD8[3]);
        func_8006E09C(&mD8[4], S32(arg0, 0x178) / 12);
        func_8006E108(&mD8[5], S32(arg0, 0x17C));
        func_80090DB4(&m30, &mD8[4]);
        func_80090DB4(&m10, &mD8[4]);
        func_80090DB4(&mD8[4], &mD8[5]);
        func_80090CA4(&m90, &m30);
        RR_GTE_SETROT(&m90);
        for (i = 0; i < 2; i++) {
            /* ASM_BARRIER: pin x to v0 to match branch delay slot and load scheduling */
            register s32 x asm("v0") = D_800F3A08[0][0];
            if (i & 1) {
                x = -x;
            }
            svD0.vx = x << 2;
            /* ASM_BARRIER: force reload of D_800F3A08[0] after storing svD0.vx */
            __asm__ volatile("" : : : "memory");
            svD0.vy = (u16)D_800F3A08[0][1] << 2;
            svD0.vz = -((u16)D_800F3A08[0][2]) << 2;
            RR_GTE_MVMVA_ROT_V0_NOCV(&svD0);
            RR_GTE_STORE_IR(&v198[i + 3]);
            v198[i + 3].vx += v198[2].vx;
            v198[i + 3].vy += v198[2].vy;
            v198[i + 3].vz += v198[2].vz;
        }
        func_8006EE3C(0x1F80007C, &v198[0], &mD8[0]);
        a1 = 1;
        G->col78 = 0x05000000;
        if (D_800F2FD8 >= 4) {
            a1 = 3;
        }
        func_8005F0D0(0x1F800000, a1, 0, s5);
        func_8006EE3C(0x1F80007C, &v198[1], &mD8[1]);
        G->col78 = 0x05000000;
        func_8005F0D0(0x1F800000, D_800F2FD8 < 1, arg6, s5);
        func_8006EE3C(0x1F80007C, &v198[2], &mD8[2]);
        a1 = 1;
        G->col78 = 0x05000000;
        if (D_800F2FD8 >= 10) {
            a1 = 9;
        }
        func_8005F0D0(0x1F800000, a1, 0, s5);
        func_8006EE3C(0x1F80007C, &v198[2], &mD8[3]);
        a1 = 1;
        G->col78 = 0x05000000;
        if (D_800F2FD8 >= 7) {
            a1 = 6;
        }
        func_8005F0D0(0x1F800000, a1, arg6, s5);
        func_8006EE3C(0x1F80007C, &v198[3], &mD8[4]);
        a1 = 1;
        G->col78 = 0x05000000;
        if (D_800F2FD8 >= 9) {
            a1 = 8;
        }
        func_8005F0D0(0x1F800000, a1, 0, s5);
        func_8006EE3C(0x1F80007C, &v198[4], &mD8[4]);
        a1 = 1;
        G->col78 = 0x05000000;
        if (D_800F2FD8 >= 8) {
            a1 = 7;
        }
        func_8005F0D0(0x1F800000, a1, 0, s5);
        func_8006EE3C(0x1F80007C, &v198[3], &mD8[5]);
        a1 = 1;
        G->col78 = 0x05000000;
        if (D_800F2FD8 >= 6) {
            a1 = 5;
        }
        func_8005F0D0(0x1F800000, a1, arg6, s5);
        func_8006EE3C(0x1F80007C, &v198[4], &mD8[5]);
        a1 = 1;
        G->col78 = 0x05000000;
        if (D_800F2FD8 >= 5) {
            a1 = 4;
        }
        func_8005F0D0(0x1F800000, a1, arg6, s5);
    } else if (i < arg4) {
        s5 = 0;
        if (G->flag64 != 0) {
            s5 = -0x60;
        }
        func_8006E09C(&m90, -S32(arg0, 0x54));
        func_8006E108(&m50, -S32(arg0, 0x50));
        func_80090DB4(&m90, &m50);
        func_8006E030(&m70, -S32(arg0, 0x58));
        func_80090DB4(&m50, &m70);
        func_80090914(D_800ABED0, &m70, &mB0);
        func_800910E4(&mB0);
        func_8006E09C(&m10, -S32(arg0, 0x54));
        func_80090DB4((MATRIX *)0x1F800028, &m10);
        func_8006E108(&mD8[0], S16(arg0, 0x1A8) - S32(arg0, 0x50));
        func_8006E030(&m50, S16(arg0, 0x1AA) - S32(arg0, 0x58));
        func_80090CA4(&mD8[0], &m50);
        func_80090DB4(&m10, &mD8[0]);
        v198[0] = VEC(arg0, 0xC8);
        v198[1].vx = S32(arg0, 0x70) + S32(arg0, 0x60);
        v198[1].vy = S32(arg0, 0x74) + S32(arg0, 0x64);
        v198[1].vz = S32(arg0, 0x78) + S32(arg0, 0x68);
        func_8006E108(&mD8[1], -S16(arg0, 0x80));
        func_8006E030(&m50, -S16(arg0, 0x84));
        func_80090CA4(&mD8[1], &m50);
        func_80090DB4(&m10, &mD8[1]);
        func_8006EE3C(0x1F80007C, &v198[0], &mD8[0]);
        a1 = 1;
        G->col78 = 0;
        if (D_800F2FD8 >= 4) {
            a1 = 3;
        }
        func_8005F0D0(0x1F800000, a1, 0, s5);
        func_8006EE3C(0x1F80007C, &v198[1], &mD8[1]);
        G->col78 = 0;
        func_8005F0D0(0x1F800000, 1, arg6, s5);
    } else if (i < arg5) {
        s5 = 0;
        if (G->flag64 != 0) {
            s5 = -0x7F;
        }
        func_8006E09C(&m50, -S32(arg0, 0x54));
        func_8006E108(&m90, -S32(arg0, 0x50));
        func_80090DB4(&m50, &m90);
        func_80090914(D_800ABED0, &m90, &mB0);
        func_800910E4(&mB0);
        func_8006E030(&m50, -S32(arg0, 0x58));
        func_80090DB4(&m90, &m50);
        func_80090DB4((MATRIX *)0x1F800028, &m50);
        func_8006EE3C(0x1F80007C, arg0 + 0x20, &m50);
        a1 = 1;
        G->col78 = 0;
        if (D_800F2FD8 >= 3) {
            a1 = 2;
        }
        func_8005F0D0(0x1F800000, a1, 0, s5);
    }
    return dist;
}
