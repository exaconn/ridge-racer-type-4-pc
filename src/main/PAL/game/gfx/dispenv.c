#include "types.h"
#include "scratchpad.h"
#include "m2c_macros.h"
#include "rr/overlay662_distance.h"

struct S {
    char p0[0x1C];
    int f1C, f20, f24, f28;
    char p2C[0x0C];
    int f38, f3C, f40, f44;
};

extern int func_80043AB0();
extern struct S D_800AC650;
/* same symbol under a 2nd C name: cse cannot reuse the earlier address, so the else arm re-materialises lui/addiu like the original */
extern struct S D_800AC650b __asm__("D_800AC650");
extern int D_800AC690;
extern int D_800AC694;
extern int func_80047610();
extern int D_800ABEA4;
extern int D_800ABECC;
extern void ApplyMatrix();
extern void func_80091D64();
extern void *D_800ACBA0;
extern int func_80055330();
extern int func_8005534C();
extern int D_800ACBA8;
extern char D_8010B2E0[];

int func_80043C14();
void func_80043DE4();
void func_80043ED4(short *a, short b, int c);
void func_80043F5C(short *a, short b, int c);
short func_80043FC0(short *p, int t);
s32 func_80044064(void);
void func_800440A0(int a0);

int func_80043C14(a0, a1)
char *a0;
int *a1;
{
    int d[3];
    int sp20[4];
    int s;
    struct S *q;
    int t;
    d[0] = (D_800AC650.f20 / 4 - a1[0]) / 4;
    d[1] = (D_800AC650.f24 / 4 - a1[1]) / 4;
    d[2] = (D_800AC650.f28 / 4 - a1[2]) / 4;
    s = func_80090758(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]) * 2;
    if (D_800AC650.f1C != 0) {
        t = s >> 2;
        D_800AC650.f3C = t;
        D_800AC650.f38 = s - t;
        D_800AC650.f44 = 0;
        D_800AC650.f40 = s;
        if (s > 0xFFFF) {
            if (*(short *)(a0 + 0x1EA) != 0) goto L;
        } else {
L:
            D_800AC650.f44 = 1;
        }
        if (*(short *)((char *)a1 + 0x32) - *(int *)(a0 + 0xF0) >= 0) D_800AC694 = 1;
    }
    t = s - D_800AC650.f3C;
    if (D_800AC650.f44 == 0) {
        if (t < 0) {
            D_800AC650.f44 = 1;
        } else {
            q = &D_800AC650b;
            if (q->f40 - s < 0) q->f44 = 1;
        }
    }
    func_80043AB0(a0, sp20, a1[5]);
    return D_800AC690 = s;
}

void func_80043DE4(a0, a1, a2) char *a0; int *a1; int *a2; {
    int d;
    int q2;
    int r;
    int t;
    int j;
    d = (*(int *)(a0 + 0xF0) << 10) + *(short *)(a0 + 0x170) - a2[2];
    t = d;
    if (d < 0) t = d + 0x3FF;
    r = t >> 10;
    q2 = r;
    r = d - (q2 << 10);
    d = q2;
    if (d < 0) d += D_800ABECC;
    j = d + 1;
    if (D_800ABECC < j) j -= D_800ABECC;
    func_80047610(D_800ABEA4 + d * 60, D_800ABEA4 + j * 60, a2[0], r, a1);
    a1[0] = a1[0] << 2;
    a1[2] = a1[2] << 2;
    a1[1] = a1[1] + (a2[1] << 2);
}

void func_80043ED4(short *a, short b, int c) {
    int m[8];
    unsigned short v[3];
    short r[3];
    int *s = (int *)SCRATCHPAD;
    s[6] = -a[0];
    s[7] = a[1];
    s[8] = a[2];
    v[0] = a[0];
    v[1] = a[1];
    v[2] = 0;
    r[0] = 0;
    r[1] = 0;
    r[2] = -b;
    func_80091D64(v, m);
    ApplyMatrix(m, r, c);
}

void func_80043F5C(short *a, short b, int c) {
    int m[8];
    unsigned short v[3];
    short r[3];
    v[0] = a[0];
    v[1] = a[1];
    v[2] = 0;
    r[0] = 0;
    r[1] = 0;
    r[2] = -b;
    func_80091D64(v, m);
    ApplyMatrix(m, r, c);
}

short func_80043FC0(short *p, int t) {
    int n = p[0x24 / 2] * 5;
    int v;
    if (n > 0 && t < n) {
        v = (p[0x22 / 2] - p[0x20 / 2]) * t / n + p[0x20 / 2];
    } else {
        v = p[0x22 / 2];
    }
    return v - (v / 4096 * 4096);
}

s32 func_80044064(void) {
    s32 var_a1;
    void *var_a0;

    var_a0 = D_800ACBA0;
    var_a1 = 0;
    if (M2C_FIELD(var_a0, s16 *, 0x32) != -1) {
        do {
            var_a0 += 0x44;
            var_a1 += 1;
        } while (M2C_FIELD(var_a0, s16 *, 0x32) != -1);
    }
    return var_a1;
}

void func_800440A0(int a0) {
    int s2, s0, x, s1, v1, y;
    int *v0;
    s1 = D_800ACBA8;
    s2 = 0;
    s0 = -1;
    a0 = ((unsigned)a0 >> 4) & 0xFF;
    /* ASM_BARRIER: legacy, reason not recorded (forces reg copy/CSE/scheduling); retire & retest with fd.py */
    __asm__("" : "=r"(x) : "0"(a0));
    if (a0 < 8) {
        s0 = (a0 - 1) % 4;
    }
    if (x >= 5) {
        s2 = 1;
    }
    if (s0 >= 0) {
        func_8005534C(1, x);
        s1 += s0 * 8;
        func_80055330(*(int *)s1, *(unsigned char *)(s1 + 4), *(unsigned char *)(s1 + 5), *(unsigned char *)(s1 + 6));
        v1 = 7;
    } else {
        func_8005534C(0, x);
        v1 = 7;
    }
    v0 = (int *)D_8010B2E0;
    v0 = (int *)((char *)v0 + 0x578);
    do {
        v0[0x70 / 4] = s2;
        v1--;
        v0 = (int *)((char *)v0 - 0xC8);
    } while (v1 >= 0);
}
