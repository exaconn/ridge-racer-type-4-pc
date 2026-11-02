extern int func_8002B75C();
extern int func_80078414();
extern int func_80078E40();
extern int func_8007A9CC();
extern int D_800A45C0;
extern unsigned char D_800ACBCC;
extern unsigned char D_800F3A24[];
extern char D_800A457C[];
extern char D_800F4D50[];
extern short D_800F3AF8;

int func_8007CDC4(a0)
int a0;
{
    int ret;
    int i;
    int v;
    char *p;
    if (a0 == 0) {
        D_800A45C0 = 0;
    }
    ret = 0;
    switch (D_800A45C0) {
    case 0:
        D_800F3A24[1] = 0x5A;
        D_800F3A24[0] = 0x5A;
        func_80078E40(0, 0, 1);
        i = 0x17;
        p = D_800F4D50 + i;
        do {
            *p = 0;
            i--;
            p--;
        } while (i >= 0);
        func_8002B75C(D_800F4D50, D_800A457C);
        D_800F3AF8 = 0x30;
        D_800A45C0 = 1;
        break;
    case 1:
        func_80078E40(0, 0, 0);
        if (D_800F3A24[0] < 0x10U) {
            if (func_80078414(D_800ACBCC, D_800F4D50) == 1) {
                func_8007A9CC(2, 0);
                D_800A45C0 = 2;
            } else {
                goto set1;
            }
            break;
        }
        v = D_800F3A24[0];
        switch (v) {
        case 0x14:
        case 0x1E:
        case 0x28:
        case 0x3C:
            D_800A45C0 = 3;
            goto set1;
        case 0x32:
        case 0x5A:
            ret = 0;
            break;
        }
        break;
    case 2:
        ret = func_8007A9CC(0, 0);
        if (ret == 4) {
            goto set1;
        } else if (ret == 5) {
            goto set1;
        } else if (ret == 6) {
            goto set1;
        } else {
            ret = 0;
        }
        break;
    case 3:
set1:
        ret = 1;
        break;
    default:
        return ret;
    }
    return ret;
}
