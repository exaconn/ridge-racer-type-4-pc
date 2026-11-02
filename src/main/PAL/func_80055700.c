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
#define P D_800AC650.p
#define B(o) (*(unsigned char *)(P + (o)))
#define H(o) (*(short *)(P + (o)))
#define W(o) (*(int *)(P + (o)))

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
