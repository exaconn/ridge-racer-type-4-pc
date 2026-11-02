#include "rr_asm.h"
extern int func_8006E09C();
extern int func_8006E108();
extern int func_8006E030();
extern int func_80090CA4();

#define S(b, o) (*(short *)((b) + (o)))
#define U(b, o) (*(unsigned short *)((b) + (o)))
#define I(b, o) (*(int *)((b) + (o)))

void func_800244B0(char *p) {
    char *s5;
    char *s2;
    char *s1;
    char *s0;
    int s3;
    int v1, v0, a1, a0, m1, m2;

    s5 = (char *)0x1F800080;
    if ((U(p, 0x1E4) & 0x10) == 0) {
        m1 = S(p, 0x10E) * S(p, 0x1D8);
        I(p, 0x30) = I(p, 0x30) << 8;
        I(p, 0x38) = I(p, 0x38) << 8;
        S(s5, 0x68) = m1 / (I(p, 0x2EC) / 128 + 1);
    } else {
        m2 = S(p, 0x10E) * S(p, 0x1D8);
        v0 = I(p, 0x28C) + 1;
        S(s5, 0x68) = (m2 / v0) / 2;
        S(p, 0x1DA) = U(p, 0x1D8);
        S(p, 0x1D6) = U(p, 0x1D4);
    }
    s2 = s5 + 0x24;
    s3 = 0x800;
    a1 = s3 - I(p, 0x54);
    I(p, 0) = I(p, 0) + I(p, 0x30);
    I(p, 8) = I(p, 8) + I(p, 0x38);
    I(p, 0x10) = I(p, 0) >> 6;
    I(p, 0x18) = I(p, 8) >> 6;
    func_8006E09C(s2, a1);
    s0 = s5 + 4;
    func_8006E108(s0, I(p, 0x50));
    func_80090CA4(s2, s0);
    func_8006E030(s0, I(p, 0x58));
    func_80090CA4(s2, s0);
    s1 = s5 + 0x44;
    func_8006E09C(s1, s3 - S(p, 0x82));
    func_8006E108(s0, S(p, 0x80));
    func_80090CA4(s1, s0);
    func_8006E030(s0, S(p, 0x84));
    func_80090CA4(s1, s0);
    S(s5, 0x64) = 0;
    S(s5, 0x66) = -U(p, 0x108);
    RR_GTE_SETROT(s2);
    RR_GTE_MVMVA_ROT_V0_NOCV(s5 + 0x64);
    S(s5, 0x6C) = 0;
    v0 = S(p, 0xE8);
    v1 = S(p, 0xEA);
    S(s5, 0x70) = 0;
    S(s5, 0x6E) = (v0 + v1) >> 7;
    RR_GTE_STORE_IR(p + 0x40);
    RR_GTE_SETROT(s1);
    RR_GTE_MVMVA_ROT_V0_NOCV(s5 + 0x6C);
    S(s5, 0x74) = 0;
    v0 = S(p, 0xE0);
    v1 = S(p, 0xE2);
    S(s5, 0x78) = 0;
    S(s5, 0x76) = (v0 + v1) >> 3;
    RR_GTE_STORE_IR(p + 0x70);
    RR_GTE_MVMVA_ROT_V0_NOCV(s5 + 0x74);
    I(p, 0x10) = I(p, 0x10) + I(p, 0x40);
    I(p, 0x18) = I(p, 0x18) + I(p, 0x48);
    I(p, 0x20) = I(p, 0x10) >> 2;
    I(p, 0x28) = I(p, 0x18) >> 2;
    RR_GTE_STORE_IR(p + 0x60);
}
