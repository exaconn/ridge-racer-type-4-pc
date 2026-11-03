/* RODATA 0x80010468 0x20 */
extern int func_80090424();
extern int func_8004F218();
extern int func_8004B7E0();
extern char D_800ABEF8[];
extern int D_800AD504;

#define B(o) (*(unsigned char *)(p + (o)))
#define S(o) (*(short *)(p + (o)))
#define U(o) (*(unsigned short *)(p + (o)))
#define I(o) (*(int *)(p + (o)))

void func_8002C328(char *p) {
    int s1, s2, a1, v;

    if (B(0x1A4)) {
        B(0x1A5) = B(0x1A5) - 1;
        if (B(0x1A5) == 0) {
            B(0x1A4) = 0;
            S(0x1B4) = 0;
            S(0x1B6) = 0;
            S(0x1B0) = 0;
            S(0x1B2) = 0;
        }
        a1 = B(0x1A5);
        s1 = (a1 * S(0x1A2)) / 128;
        v = func_80090424((a1 * 0x3000) / 30);
        s2 = (v * s1) / 2048;
        switch (B(0x1A4)) {
        case 1:
            if (p == D_800ABEF8 || p == D_800ABEF8 + 0x320) {
                if (D_800AD504 != 2 && B(0x1A5) == 0x1D) {
                    a1 = S(0x1BA) * 4;
                    if (a1 > 0x64) a1 = 0x64;
                    func_8004F218(0x28, (short)a1, 0x40, -1);
                }
            }
        case 5:
            S(0x1B0) = s2 + s1;
            S(0x1B2) = (s2 + s1 / 2) / 2;
            a1 = S(0x1B0);
            if (a1 < 0) a1 += 3;
            a1 >>= 2;
            U(0x1AC) = U(0x1AC) + a1;
            U(0x1AE) = U(0x1AE) + U(0x1B2);
            break;
        case 2:
        case 8:
            if (S(0x1B8) == 0) S(0x1B6) = s2;
            U(0x1AE) = U(0x1AE) + U(0x1B6);
            break;
        case 3:
            v = s2 + s1;
            S(0x1B4) = v;
            U(0x1AC) = U(0x1AC) + v;
            break;
        }
    } else if (B(0x1A7)) {
        if (p == D_800ABEF8 || p == D_800ABEF8 + 0x320) {
            func_8004B7E0(I(0x218) & 1, 1, 1);
        }
        s1 = (short)U(0x1D8) >> 2;
        a1 = B(0x1A7);
        s1 = (a1 * s1) / 128;
        v = func_80090424((a1 * 0x3000) / 30);
        s2 = (v * s1) / 2048;
        if (S(0x1B8) == 0) S(0x1B6) = s2;
        U(0x1AE) = U(0x1AE) + U(0x1B6);
        B(0x1A7) = B(0x1A7) - 1;
    } else {
        S(0x1B6) = 0;
        if (p == D_800ABEF8 || p == D_800ABEF8 + 0x320) {
            func_8004B7E0(I(0x218) & 1, 0, 1);
        }
    }
}
