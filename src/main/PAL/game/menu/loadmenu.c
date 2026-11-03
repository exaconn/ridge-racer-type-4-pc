/* RODATA 0x80011588 0x1c */
extern int func_80078178();
extern int func_80078490();
extern int func_80078544();
extern int func_800797E0();
extern short D_800A41EC;
extern short D_800A41EE;
extern short D_800F3AF8;
extern char D_800F4D50[];
extern int D_800ACC0C;
extern short D_800F3A26;
extern short D_800F4C6A;
extern short D_800F4D8A;

int func_8007A9CC();
void func_8007AB8C(int arg0);

int func_8007A9CC(a0, a1)
short a0;
int a1;
{
    int i;
    int r;
    int q;
    if (a0 > 0) {
        D_800A41EC = 0;
    }
    switch (D_800A41EC) {
    case 0:
        D_800A41EE = 0;
        D_800A41EC = 1;
        break;
    case 1:
        D_800A41EC = 2;
        break;
    case 2:
        for (i = 0x1D; i >= 0; i--) {
        }
        if (D_800F3AF8 != 0x10) {
            r = func_80078490((short)a1, 0, D_800F4D50, D_800ACC0C, 1);
        } else {
            r = func_80078544((short)a1, 0, D_800F4D50, D_800ACC0C, 0x80, 0x280);
        }
        if (r == -1) {
            if (D_800A41EE < 3) {
                D_800A41EE = (unsigned short)D_800A41EE + 1;
                break;
            }
            D_800A41EC = 5;
        } else {
            D_800A41EC = 3;
        }
        break;
    case 3:
        q = func_80078178();
        if (q != 0) {
            if (q == 1) {
                if (func_800797E0(D_800F3AF8) != 0) {
                    D_800A41EC = 4;
                } else {
                    D_800A41EC = 6;
                }
            } else {
                if (D_800A41EE >= 3) {
                    D_800A41EC = 5;
                } else {
                    D_800A41EC = 1;
                    D_800A41EE = (unsigned short)D_800A41EE + 1;
                }
            }
        }
        break;
    case 4:
    case 5:
    case 6:
        break;
    }
    return D_800A41EC;
}

void func_8007AB8C(int arg0) {
    if (arg0 == -1) {
        D_800F4C6A = 1;
        D_800F3A26 = 3;
    } else {
        D_800F4C6A = 1;
        D_800F3A26 = 0xF;
    }
    D_800F4D8A = 0;
}
