/* RODATA 0x8001164c 0x20 */
extern int func_8002B75C();
extern int func_80078178();
extern int func_80078414();
extern int func_80078544();
extern int func_80078E40();
extern short D_800A45BC;
extern short D_800A45BE;
extern unsigned char D_800ACBCC;
extern int D_800ACC0C;
extern unsigned char D_800F3A24[];
extern char D_800A457C[];
extern char D_800F4D50[];
extern short D_800FFC00;

int func_8007CB44(a0)
int a0;
{
    int pad[14];
    int ret;
    int i;
    int r;
    int q;
    int v;
    if (a0 == 0) {
        D_800A45BC = 0;
    }
    ret = 0;
    switch (D_800A45BC) {
    case 0:
        D_800F3A24[1] = 0x5A;
        D_800F3A24[0] = 0x5A;
        func_80078E40(D_800FFC00, 0, 1);
        func_8002B75C(D_800F4D50, D_800A457C);
        D_800A45BC = 1;
        break;
    case 1:
        func_80078E40(D_800FFC00, 0, 0);
        if (D_800F3A24[0] < 0x10U) {
            if (func_80078414(D_800ACBCC, D_800F4D50) == 1) {
                D_800A45BE = 0;
                D_800A45BC = 2;
            } else {
                D_800A45BC = 7;
            }
            break;
        }
        v = D_800F3A24[0];
        switch (v) {
        case 0x14:
        case 0x1E:
        case 0x28:
        case 0x3C:
            D_800A45BC = 7;
            break;
        case 0x32:
        case 0x5A:
            ret = 0;
            break;
        }
        break;
    case 2:
        D_800A45BC = 3;
        break;
    case 3:
        for (i = 0x63; i >= 0; i--) {
        }
        r = func_80078544(D_800FFC00, 0, D_800F4D50, D_800ACC0C + 0x1F00, 0x100, 0x300);
        if (r == -1) {
            if (D_800A45BE < 3) {
                D_800A45BE = (unsigned short)D_800A45BE + 1;
            } else {
                D_800A45BC = 7;
            }
        } else {
            D_800A45BC = 4;
        }
        break;
    case 4:
        for (i = 0x63; i >= 0; i--) {
        }
        q = func_80078178();
        if (q != 0) {
            if (q == 1) {
                D_800A45BC = 5;
            } else if (D_800A45BE >= 3) {
                D_800A45BC = 7;
            } else {
                D_800A45BC = 2;
                D_800A45BE = (unsigned short)D_800A45BE + 1;
            }
        }
        break;
    case 5:
        ret = 1;
        break;
    case 6:
    case 7:
    default:
        ret = -1;
        break;
    }
    return ret;
}
