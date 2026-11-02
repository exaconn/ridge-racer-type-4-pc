/* RODATA 0x8001166c 0x14 */
extern int func_8002B75C();
extern int func_80078414();
extern int func_80078E40();
extern int func_8007A9CC();
extern int D_800A45C4;
extern unsigned char D_800ACBCC;
extern unsigned char D_800F3A24[];
extern char *D_800A4594[];
extern char D_800F4D50[];
extern short D_800F3AF8;

int func_8007CFC0(a0, a1)
int a0, a1;
{
    int ret;
    int v;
    if (a0 == 0) {
        D_800A45C4 = 0;
    }
    ret = 0;
    switch (D_800A45C4) {
    default:
    case 0:
        D_800F3A24[1] = 0x5A;
        D_800F3A24[0] = 0x5A;
        func_80078E40(0, 0, 1);
        D_800A45C4 = 1;
        func_8002B75C(D_800F4D50, D_800A4594[a1]);
        D_800F3AF8 = 0x10;
        break;
    case 1:
        func_80078E40(0, 0, 0);
        if (D_800F3A24[0] < 0x10U) {
            if (func_80078414(D_800ACBCC, D_800F4D50) == 1) {
                func_8007A9CC(2, 0);
                D_800A45C4 = 2;
            } else {
                ret = -1;
            }
            break;
        }
        v = D_800F3A24[0];
        switch (v) {
        case 0x14:
        case 0x1E:
        case 0x28:
        case 0x3C:
            D_800A45C4 = 3;
            ret = -2;
            break;
        case 0x32:
        case 0x5A:
            ret = 0;
            break;
        }
        break;
    case 2:
        ret = func_8007A9CC(0, 0);
        if (ret == 4) {
            D_800A45C4 = ret;
            ret = 2;
        } else if (ret == 5) {
            ret = -1;
        } else if (ret == 6) {
            ret = -1;
        } else {
            ret = 0;
        }
        break;
    case 3:
        ret = -1;
        break;
    case 4:
        ret = 3;
        break;
    }
    return ret;
}
