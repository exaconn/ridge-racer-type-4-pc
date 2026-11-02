extern unsigned char D_800A4478[];
extern char *D_800AC590;
extern short *D_800F3AFC;
extern short D_800F4D3C, D_800FFC00, D_800FFC18, D_80109490, D_801094A0;
extern unsigned char D_8010B2DC[];
extern int func_80032A14(), func_8007D448(), func_8007D560(), func_8007D65C();
void func_8007D1B8(a0, a1, a2, a3, a4, a5) int a0; int a1; int a2; int a3; int a4; int a5; {
    unsigned char *v1;
    int s0 = a1;
    if (a2 != 0) {
        int c14 = 0x14, c16 = 0x16, c4 = 4, c1 = 1;
        v1 = D_800A4478;
        a3 = (int)v1 + c14;
L1:
        if (a5 != v1[1]) goto next;
        if (a5 == c14) goto done;
        if (a0 != c16 || s0 != c4 || a2 != c1) s0 = 0;
        a0 = v1[0];
        goto done;
next:
        v1 += 4;
        if ((int)v1 < a3) goto L1;
done:;
        if (a0 == 5) {
            if (D_801094A0 == 1) {
                a0 = 0x1A;
            } else if (D_800FFC18 == 2 && D_800FFC00 == 1) {
                a0 = 0x21;
            }
        }
    }
    if (D_800F3AFC[8] == 3) {
        if (a0 == -1) {
            if (D_800FFC18 == 3 || D_800FFC18 == 6 || D_800FFC18 == 7) a0 = 0x19;
        } else if (a0 == -128) {
            a0 = (signed char)D_800AC590[4];
        } else {
            if (a0 == 0x12 && D_801094A0 == 1 && D_8010B2DC[D_800FFC00] == 1) {
                a0 = 0x1C;
            } else if (a0 == 0x15 && D_800FFC00 == 1) {
                a0 = 0x22;
            } else if (a0 == 0x25 && D_800FFC00 == 1) {
                a0 = 0x26;
            } else if (a0 == 0x1F && D_80109490 == 0) {
                a0 = 0x20;
            }
        }
        if (a0 >= 0) {
            D_800AC590[4] = a0;
            func_80032A14(D_800F3AFC[4] + 10, D_800F3AFC[5] + 0x28, D_800AC590, a3);
        }
    }
    func_8007D448(a4, ((s0 & 2) > 0) & (*(short *)(D_800AC590 + 0x12) >= 6));
    func_8007D560(a4, (s0 & 4) > 0);
    if (s0 & 8) func_8007D65C(D_800F4D3C);
}
