extern int func_80096464();
extern int func_80056F04();
extern int func_80057040();
extern struct { char pad[0x24C]; struct { char pad[0x140]; int v; } *p; } D_800AC650;
extern short D_800A0F9C[];
extern char *D_800AC89C;
extern int func_80057138();
extern int func_800572A8();

void func_8005748C();
void func_80057750();

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

void func_80057750() {
    int a0, a1, lo, l1, l2, a2, a3;
    lo = D_800A0F9C[0];
    a0 = *(int *)(D_800AC89C + 0x104);
    if (a0 >= lo) {
        if (a0 < D_800A0F9C[1]) {
            func_80057138(a0 - lo, D_800A0F9C[1] - lo - 0x20, lo);
        }
    }
    l2 = D_800A0F9C[2];
    a2 = *(int *)(D_800AC89C + 0x104);
    if (a2 >= l2) {
        if (a2 < D_800A0F9C[3]) {
            func_800572A8(a2 - l2, D_800A0F9C[3] - l2 - 0x20);
        }
    }
    l1 = D_800A0F9C[4];
    a1 = *(int *)(D_800AC89C + 0x104);
    if (a1 >= l1) {
        if (a1 < D_800A0F9C[5]) {
            func_8005748C(0, a1 - l1, D_800A0F9C[5] - l1 - 0x20);
        }
    }
    l1 = D_800A0F9C[6];
    a1 = *(int *)(D_800AC89C + 0x104);
    if (a1 >= l1) {
        if (a1 < D_800A0F9C[7]) {
            func_8005748C(1, a1 - l1, D_800A0F9C[7] - l1 - 0x20);
        }
    }
    l1 = D_800A0F9C[8];
    a1 = *(int *)(D_800AC89C + 0x104);
    if (a1 >= l1) {
        if (a1 < D_800A0F9C[9]) {
            func_8005748C(2, a1 - l1, D_800A0F9C[9] - l1 - 0x20);
        }
    }
}
