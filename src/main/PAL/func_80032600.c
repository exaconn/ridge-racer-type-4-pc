extern int func_800207D4(), func_8002A344(), func_8002B330(), func_80032A14(), func_8003A198();
extern int func_8003B398(), func_8003B934(), func_8003D6F0(), func_8003EC88(), func_8004F24C();
extern short D_800AC5D0, D_800AC64A, D_800F3A26, D_800F4C6A;
extern unsigned short D_800F4D8A;
extern int D_800ACC14, D_800ACC18, D_800F2DE4, D_800F3A60, D_800F4D74;
extern struct { char pad[8]; short s8, sA; char padC[4]; short s10; } *D_800F3AFC;
extern struct w3g2e { char pad[0xC]; char c; char pad2[7]; unsigned char b14; char pad3[0x23]; char b38; } *D_800F4D80;
extern struct { char pad[4]; char b4; char pad5[2]; char b7; } *D_800FF68C;

void func_80032600(void) {
    int r;
    D_800F2DE4 += 1;
    switch (D_800F4D8A) {
    case 0:
        *((char *)D_800F4D80 + D_800AC64A * 0x24 + 0xC) = 0;
        func_8002B330(0, 2, &D_800AC64A, 2, 0);
        *((char *)D_800F4D80 + D_800AC64A * 0x24 + 0xC) = 2;
        if (D_800F3A60 != 0) {
            if (D_800F4D80->b14 == 2) {
                func_8004F24C(1);
                D_800F4D80->b14 = 3;
                D_800F4D80->b38 = 3;
                D_800F3AFC->s10 = 4;
                *((char *)D_800F4D80 + D_800AC64A * 0x24 + 0xC) = 1;
                D_800AC5D0 = 0x40;
                D_800ACC18 = D_800F4D74;
                func_8003A198();
                if (D_800AC64A == 0) {
                    D_800F4D8A = 3;
                } else if (D_800AC64A == 1) {
                    D_800F4D8A = 4;
                }
            }
        }
        break;
    case 3:
        if (D_800AC5D0 > 0) D_800AC5D0 = D_800AC5D0 - 1;
        func_800207D4(D_800ACC14 + 0x70, (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            r = func_8003B934();
            if (r == 1) {
                D_800F4C6A = r;
                D_800F3A26 = 0x47;
                D_800F4D8A = 0;
            }
        }
        break;
    case 4:
        if (D_800AC5D0 > 0) D_800AC5D0 = D_800AC5D0 - 1;
        func_800207D4(D_800ACC14 + 0x70, (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            r = func_8003B398(1);
            if (r == 1) {
                D_800F4C6A = r;
                D_800F3A26 = 0xF;
                D_800F4D8A = 0;
            }
        }
        break;
    }
    if (D_800F3AFC->s10 == 3) {
        D_800FF68C->b7 = 0;
        D_800FF68C->b4 = 0x26;
        func_80032A14(D_800F3AFC->s8 + 0xA, D_800F3AFC->sA + 0x28, D_800FF68C);
    }
    func_8003D6F0(0);
    func_8003EC88(0);
    func_8003EC88(1);
    func_8002A344(0x10084);
}
