#include "types.h"
#include "rr/matrix_words.h"

#define P D_800AC650.p
#define B(o) (*(unsigned char *)(P + (o)))
#define H(o) (*(short *)(P + (o)))
#define W(o) (*(int *)(P + (o)))

typedef struct { int w[4]; } Q16;

typedef RrMatrixWords32 M32;

typedef struct { short m[3][3]; short pad[7]; } S70;

extern int func_80014A8C();
extern int func_80096434();
extern short D_800A0D7C;
extern int D_800A0FB0[];
extern unsigned short D_800AC006;
extern struct { char pad[0xEC]; char buf[0x160]; char *p; } D_800AC650;
extern int D_800ACBF8;
extern short D_800F3826;
extern struct { char pad[8]; short x8; } *D_800F3A08;
extern short D_800F4C28;
extern Q16 D_800111D4;
extern short D_800ABED0[][3];
extern int func_8006E030();
extern int func_8006E09C();
extern int func_8006E108();
extern int func_80090914();
extern int func_80090CA4();
extern int func_80090DB4();
extern int func_80090F74();
extern int func_800910E4();
extern int func_80091114();
extern int func_80091274();
extern int func_80090EC4();

void func_80055700();
void func_8005599C();
void func_80055BF8(int a0, int *a1, u16 *a2, int *a3);

void func_80055700(a0, a1)
int a0, a1;
{
    int t, one;
    char *q;
    P = D_800AC650.buf;
    func_80096434(P, 0x160);
    H(0x84) = 0x1C1;
    H(0x86) = -0xDF8;
    H(0x88) = -0x79A;
    B(0x78) = 0x80;
    B(0x79) = 0x80;
    B(0x7A) = 0x80;
    H(0x8C) = 0xB9A;
    H(0x8E) = -0xADB;
    H(0x90) = 0x1E2;
    B(0x7C) = 0x40;
    B(0x7D) = 0x40;
    B(0x7E) = 0x40;
    H(0x94) = -0xA4D;
    H(0x96) = -0x642;
    H(0x98) = 0xA86;
    B(0x80) = 0x20;
    B(0x81) = 0x20;
    B(0x82) = 0x20;
    B(0x9C) = 0x40;
    B(0x9D) = 0x40;
    B(0x9E) = 0x40;
    W(0x110) = 0;
    W(0x114) = -0xF0;
    W(0x118) = 0;
    H(0x120) = 0;
    H(0x122) = 0;
    H(0x124) = 0;
    W(0x12C) = 0x800;
    W(0x130) = 0x1B8;
    W(0x18) = -0xC0;
    W(0x30) = 0xC0;
    if (!(D_800A0FB0[D_800F3826] & ((one = 1) << D_800F4C28))) {
        W(0xA0) = 0xA9;
        W(0xA4) = 0;
    } else {
        W(0xA0) = 0xCA;
        W(0xA4) = 1;
    }
    q = P;
    D_800A0D7C = *(int *)(q + 0xA0) * 4;
    t = *(int *)(q + 0xA0) - 4;
    t = t - (D_800F3A08->x8 + ((short)D_800AC006 / 2));
    *(int *)(q + 0x100) = t;
    W(0x13C) = a0;
    W(0x140) = a1;
    func_80014A8C(0x19, t, &D_800AC650, q);
    D_800ACBF8 = 0;
}

void func_8005599C(a0, a1, a2, a3) char *a0; char *a1; M32 *a2; M32 *a3; {
    M32 s10, s30, s50;
    S70 s70;
    Q16 s90;
    int n;
    s90 = D_800111D4;
    func_8006E09C(&s50, -*(short *)(a0 + 0x12));
    func_80090DB4(0x1F800028, &s50);
    func_8006E108(&s10, -*(short *)(a0 + 0x10));
    func_8006E030(&s30, -*(short *)(a0 + 0x14));
    func_80090CA4(&s10, &s30);
    func_80090DB4(&s50, &s10);
    func_8006E09C(&s50, *(short *)(a0 + 0x12));
    func_8006E108(&s30, *(short *)(a0 + 0x10));
    func_80090CA4(&s50, &s30);
    func_8006E030(&s30, *(short *)(a0 + 0x14));
    func_80090DB4(&s50, &s30);
    *a3 = s30;
    n = 0;
    do {
        D_800ABED0[n][0] = *(unsigned short *)(a1 + n * 8 + 0xC);
        D_800ABED0[n][1] = *(unsigned short *)(a1 + n * 8 + 0xE);
        D_800ABED0[n][2] = *(unsigned short *)(a1 + n * 8 + 0x10);
        s70.m[0][n] = (unsigned char)a1[n * 4] * 16;
        s70.m[1][n] = (unsigned char)a1[n * 4 + 1] * 16;
        s70.m[2][n] = (unsigned char)a1[n * 4 + 2] * 16;
        n++;
    } while (n < 3);
    func_80090914(D_800ABED0, &s10, &s30);
    func_800910E4(&s30);
    func_80091114(&s70);
    func_80091274((unsigned char)a1[0x24], (unsigned char)a1[0x25], (unsigned char)a1[0x26]);
    func_80090F74(&s10, &s90);
    *a2 = s10;
}

void func_80055BF8(int a0, int *a1, u16 *a2, int *a3) {
    s16 t[3];
    t[0] = a2[0];
    t[1] = a2[2];
    t[2] = a2[4];
    func_80090EC4(a0, t, a3);
    a3[0] += a1[0];
    a3[1] += a1[1];
    a3[2] += a1[2];
}
