extern int func_80096464();
extern int func_80056F04();
extern int func_80057040();
extern struct { char pad[0x24C]; struct { char pad[0x140]; int v; } *p; } D_800AC650;

void func_8005748C(a0, a1, a2) int a0, a1, a2; {
    int s4 = 0x80;
    int s5 = 0;
    int s0 = 0;
    int s1 = 0;
    int r, q;
    if (a1 < 0x20) {
        s4 = a1 << 2;
    } else if (a2 < a1) {
        s4 -= (a1 - a2) << 2;
    }
    if (a1 < 0x30) {
        r = func_80096464();
        q = r / 16;
        q = r - q * 16;
        s0 = q - 8;
        r = func_80096464();
        q = r / 8;
        q = r - q * 8;
        s1 = q - 4;
        s5 = s4 / 4;
    }
    switch (a0) {
    case 0:
        func_80056F04(0x1A, 0, (short)(s0 + 0xD8), s4, 1);
        func_80056F04(0x1A, 0, (short)(s1 + 0xD8), s5, 1);
        break;
    case 1:
        func_80056F04(0xE, 0, (short)(s0 + 0xD8), s4, 1);
        func_80056F04(0xE, 0, (short)(s1 + 0xD8), s5, 1);
        s0 = (short)(s0 + 0xD8);
        s1 = (short)(s1 + 0xD8);
        func_80057040(D_800AC650.p->v / 10, 0xA4, s0, s4);
        func_80057040(D_800AC650.p->v / 10, 0xA4, s1, s5);
        func_80057040(D_800AC650.p->v % 10, 0xA8, s0, s4);
        func_80057040(D_800AC650.p->v % 10, 0xA8, s1, s5);
        break;
    case 2:
        func_80056F04(0x1B, 0, (short)(s0 + 0xD8), s4, 1);
        func_80056F04(0x1B, 0, (short)(s1 + 0xD8), s5, 1);
        break;
    }
}
