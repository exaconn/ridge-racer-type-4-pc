extern int func_8002B330();
extern int func_8002B75C();
extern int func_8004F24C();
extern int func_80078E40();
extern short D_800FFC18;
extern short D_800FFC00;
extern short D_800AC594;
extern short D_800F3A1C;
extern short D_800F4D3C;
extern short D_800F3AF8;
extern short D_801094A0;
extern char D_800F4D50[];
extern char D_800A45A0[];
extern char D_800A457C[];
extern int D_800A4594[];
extern struct { char a[4]; unsigned char b; char c[3]; } D_800A4258[];
extern unsigned char D_800A448C[][4];
extern struct { char pad[0x28]; int x28; int x2c; char pad2[0x2C]; } D_800F3A38[];

void func_8007B774(void) {
    /* ASM_BARRIER: pins value to s3 as in the original */
    register int s3 asm("$19");
    int i;
    i = D_800A4258[D_800F3A1C].b;
    s3 = D_800A448C[i][1];
    switch (D_800FFC18) {
    case 3:
    case 6:
    case 7:
        func_8002B330((D_800AC594 & 1) << 2, 2, &D_800F4D3C, 3, 0);
        if (D_800F3A38[D_800AC594].x28 != 0) {
            func_8004F24C(1);
            if ((unsigned short)(D_800FFC18 - 6) < 2) {
                D_800F3AF8 = 0x11;
                func_8002B75C(D_800F4D50, D_800A4594[D_800F4D3C]);
            } else {
                func_8002B75C(D_800F4D50, D_800A457C);
            }
        } else {
            if (D_800F3A38[D_800AC594].x2c != 0) {
                D_800F3A1C = 0;
            }
            return;
        }
        break;
    default:
        if (D_800FFC18 == 0) {
            if (D_800F3A1C == 0x3A) {
                D_801094A0 = 1;
                D_800F3AF8 = 0x20;
                func_8002B75C(D_800F4D50, D_800A45A0);
            } else {
                func_8002B75C(D_800F4D50, D_800A457C);
            }
        } else {
            func_8002B75C(D_800F4D50, D_800A457C);
        }
        break;
    }
    D_800F3A1C = s3;
    func_80078E40(D_800FFC00, 0, 1);
}
