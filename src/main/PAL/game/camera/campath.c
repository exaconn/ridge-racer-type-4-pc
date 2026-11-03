#include "types.h"

typedef struct {
    s16 vx, vy, vz, p0;
    s32 sxy;
    s32 p1;
    s32 e0, e1, e2;
    s32 p2;
    s32 z0, z1, z2;
    s32 p3;
    s16 r0, r1, r2, p4;
    char m1[0x20];
    char m2[0x20];
    s32 fl;
    s32 dep;
} w3t1_F;

extern char D_800AC650[];
extern u16 D_800A1F90[];
extern s32 func_8005599C();
extern s32 func_80090F74();
extern s32 func_8006EE3C();
extern s32 func_8005A91C();
extern s32 func_8005A9C4();
extern int D_800ACA28;
extern char D_800A1CE0[];
extern void func_80096434();

void func_8005AA2C(void);
void func_8005ABAC(int a0);

void func_8005AA2C(void) {
    w3t1_F f;
    char *b = D_800AC650;
    char *c;
    u16 *row;
    s32 i, j, x, y, v;
    c = *(char **)(b + 0x3D8);
    f.z0 = 0;
    f.z1 = 0;
    f.z2 = 0;
    f.r0 = *(u16 *)(c + 0x170);
    f.r1 = *(u16 *)(c + 0x172);
    f.r2 = *(u16 *)(c + 0x174);
    func_8005599C(&f.z0, c + 0x20, f.m1, f.m2);
    v = *(s32 *)(*(char **)(b + 0x3D8) + 0x178);
    f.e2 = v;
    f.e1 = v;
    f.e0 = v;
    func_80090F74(f.m1, &f.e0);
    func_8006EE3C(0x1F80007C, &f.z0, f.m1);
    row = D_800A1F90; i = 0; y = -0x438;
    do {
        j = 0;
        x = -0x438;
        do {
            if (*row & (0x8000 >> j)) {
                f.vx = x;
                f.vy = y;
                f.vz = 0;
                __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(&f.vx));
                __asm__ volatile ("nop\n\tnop\n\t.word 0x4A180001");
                __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(&f.sxy) : "memory");
                __asm__ volatile ("swc2 $8, 0(%0)\n\tcfc2 $12, $31\n\tnop\n\tsw $12, 0(%0)" : : "r"(&f.fl) : "$12", "memory");
                __asm__ volatile ("mfc2 $12, $19\n\tnop\n\tsra $12, $12, 2\n\tsw $12, 0(%0)" : : "r"(&f.dep) : "$12", "memory");
                f.dep = f.dep >> 3;
                func_8005A91C(((s16 *)&f.sxy)[0], ((s16 *)&f.sxy)[1], f.dep);
            }
            j++;
            x += 0x90;
        } while (j < 16);
        y += 0x90;
        i++;
        row++;
    } while (i < 16);
    func_8005A9C4();
}

void func_8005ABAC(int a0) {
    int *p = (int *)(D_800ACA28 + 0x15C);
    func_80096434(p, 0x2C);
    p[1] = 0;
    p[3] = (int)(D_800A1CE0 + (a0 << 5));
    p[7] = 0x1000;
}
