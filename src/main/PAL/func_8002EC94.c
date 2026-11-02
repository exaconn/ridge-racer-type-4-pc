/* RODATA 0x800104c4 0x2c */
typedef struct { char p0[2]; short s2; char p4[4]; short s8; short sA; char pC[4]; short s10; } W7C0_F;
extern int D_800F2DE4;
extern unsigned D_800ABEB0;
extern unsigned short D_800F4D8A;
extern short D_800F4BE0;
extern W7C0_F *D_800F3AFC;
extern int D_800F3A60;
extern unsigned char *D_800AC590;
extern unsigned short D_800F3A40;
extern short D_800AC64A;
extern unsigned char *D_800F4D80;
extern short D_800AC5D0;
extern short D_800F4D2C;
extern unsigned short D_800F4C5C;
extern short D_800F4C6A;
extern short D_800F3A26;
extern int D_8010BFBC;
extern int func_8002F0E0();
extern void func_8002F100();
extern int func_8002F1BC();
extern void func_80032A14();
extern void func_80034E14();
extern void func_8003A198();
extern int func_8003A9E8();
extern void func_8003D228();
extern void func_8003D6F0();
extern void func_8004F24C();
extern void func_8005C7A4();
extern void func_8005C8D8();
extern void func_8005CA04();
extern void func_8005D974();
extern void func_8009AEAC();

void func_8002EC94(void)
{
    int s0;
    int v1;
    int a1;
    int s0b;
    int c3, c4;
    W7C0_F *a0;

    D_800F2DE4 = D_800F2DE4 + 1;
    D_800ABEB0 = D_800ABEB0 + 1;
    s0 = func_8002F0E0();
    switch (D_800F4D8A) {
    case 0:
    case 10:
        func_8009AEAC(1, 1, 0);
        if (D_800F4BE0 > 0) {
            func_8003D6F0(0);
            if (D_800F3AFC->s10 == 3) {
                if (D_800F3A60 == 0 && D_800F4D8A != 0 && (signed char)D_800AC590[6] != 4)
                    goto call;
                if (D_800F4D8A != 0xA)
                    goto els;
                func_8004F24C(3);
                D_800AC590[4] = D_800AC590[4] + 1;
                goto c8;
            set:
                D_800AC590[4] = v1;
                goto call;
            els:
                D_800F4D8A = 0xA;
            c8:
                v1 = (signed char)D_800AC590[4];
                if (v1 < 4) {
                    c3 = 3;
                    c4 = 4;
                    a1 = D_800F4BE0;
                    a0 = D_800F3AFC;
                    do {
                        if ((a1 >> v1) & 1)
                            goto set;
                        if (v1 == c3)
                            a0->s10 = c4;
                        v1++;
                    } while (v1 < 4);
                }
            call:
                func_80032A14(D_800F3AFC->s8 + 0xA, D_800F3AFC->sA + 0x28, D_800AC590);
                return;
            } else if (D_800F3AFC->s10 == 0xB) {
                D_800F4D8A = 1;
                D_800F4BE0 = 0;
                D_800ABEB0 = 0;
                return;
            }
        } else {
            D_800F4D8A = 1;
            D_800ABEB0 = 0;
            return;
        }
        return;
    case 1:
        func_8009AEAC(1, 1, 0);
        if (D_800F3A40 & 0x800) {
            if (s0 == 0)
                func_8005C7A4();
            func_8004F24C(1);
            func_8005CA04();
            D_800AC64A = 0;
            func_80034E14();
            D_800F4D8A = 2;
            func_8003D228(0, 0x10, 0x1B6, 0x40, 0x22, 0, 0xF00);
            D_800F3AFC->s2 = 1;
        }
        if (D_800ABEB0 >= 0x2EE)
            D_800F4D8A = 5;
        return;
    case 2:
        func_80034E14();
        s0b = D_800AC64A;
        if (D_800F4D80[0x14] == 2) {
            if (func_8002F1BC() != 0) {
                D_800F4D8A = 3;
                D_800AC5D0 = 0x40;
                func_8003A198();
                func_8005C8D8(D_800F4D2C);
                D_800F3AFC->s10 = 4;
            }
        }
        if (s0b != D_800AC64A)
            D_800ABEB0 = 0;
        if (D_800ABEB0 >= 0xBB8)
            D_800F4D8A = 5;
        func_8002F100();
        func_8003D6F0(0);
        return;
    case 3:
        if (D_800AC5D0 <= 0) {
            if (func_8003A9E8() == 1) {
                D_800F4D8A = 0;
                D_800F4C6A = D_800F4C5C >> 8;
                D_800F3A26 = D_800F4C5C & 0xFF;
            }
        } else {
            D_800AC5D0 = D_800AC5D0 - 1;
        }
        func_8002F100();
        func_8003D6F0(0);
        return;
    case 5:
        func_8009AEAC(1, 1, 0);
        if (D_8010BFBC & 1) {
            D_800F4C6A = 2;
            D_800F3A26 = 3;
            D_800F4D8A = 0;
        } else {
            func_8005D974(0);
        }
        D_8010BFBC = D_8010BFBC + 1;
        if (D_8010BFBC >= 0x64)
            D_8010BFBC = 0;
        return;
    }
}
