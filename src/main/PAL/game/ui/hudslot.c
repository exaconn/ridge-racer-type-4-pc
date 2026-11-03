extern int D_800ACC14;
extern int D_800ABEB0;
extern short D_800F4D44;
extern short D_80107178;
extern short D_800AC59C;
extern short D_800AC0E6;
extern signed char D_800AD4EF;
extern unsigned char D_8009EE40[];
extern unsigned char D_8009EE44[];
extern int func_80090424();
extern int func_80095B24();

void func_80021A7C(a0, a1) int a0; int a1; {
    int base, a, s0, s1, v;
    char *s2, *p2;
    base = D_800ACC14;
    s2 = (char *)(base + 0x226B0);
    *(char *)(a1 * 0x78 + (int)s2 + 0xC) = (a0 + 1) * 0x18;
    p2 = (char *)(base + 0x226D8);
    if (D_800F4D44 == 4) {
        a = D_80107178;
    } else {
        if (D_800F4D44 == 3) {
            a = 2;
        } else {
            a = D_800AC59C;
        }
    }
    *(char *)(a1 * 0x78 + (int)p2 + 0xC) = D_8009EE44[a - 1];
    if (D_800F4D44 == 1) {
        s0 = 0x40;
        s1 = 0;
        if (D_800AC0E6 <= D_8009EE40[D_800AD4EF / 2]) {
            s1 = 1;
            v = func_80090424((D_800ABEB0 & 0x3F) << 6);
            if (v < 0) v += 0x3F;
            s0 = v >> 6;
            if (s0 < 0) {
                s1 = 0;
                s0 = -s0;
            }
        }
        s0 += 0x40;
        *(short *)(s2 + 0xE) = s1 + 0x7EF8;
        func_80095B24(s2, 0);
        s2[4] = s0;
        s2[5] = s0;
        s2[6] = s0;
    }
}
