/* RODATA 0x800104f4 0x1c */
typedef struct { char pad0[0xC]; char b0C; char pad1[7]; unsigned char b14; char pad2[0xB]; short s20; char pad3[2]; } W7C0_E;
extern short D_800AC64A;
extern short D_800AD564;
extern int D_800F3A60;
extern int D_800F3AA4;
extern short D_800F4BE8;
extern int D_800F4BEC;
extern short D_800F4C5C;
extern int D_800F4CA8;
extern short D_800F4CD0;
extern unsigned short D_800F4D2C;
extern short D_800F4D44;
extern W7C0_E *D_800F4D80;
extern short D_800FFC10;
extern int func_80034E3C();
extern void func_8002B330();
extern void func_8004F24C();
extern void func_80034E14();
extern unsigned func_8009AEAC();
extern void func_8001F290();

int func_8002F1BC(void)
{
    int s1, s2, i;
    int v;

    s1 = D_800F4CD0 + func_80034E3C();
    if (D_800AC64A == D_800F4CD0) {
        if (s1 == D_800AC64A) {
            D_800F4D80[s1].b0C = 0;
            D_800AC64A = 0;
        }
    }
    D_800F4D80[D_800AC64A].b0C = 0;
    func_8002B330(0, 2, &D_800AC64A, s1, 0);
    func_8002B330(3, 4, &D_800AD564, 5, 0);
    if (D_800AC64A == 2 && D_800F3AA4 == -1)
        D_800F4D80[2].b0C = 4;
    else
        D_800F4D80[D_800AC64A].b0C = 2;
    s2 = 0;
    if (D_800F3A60 != 0) {
        if (D_800F4D80[D_800AC64A].s20 == 1) {
            func_8004F24C(4);
            return s2;
        }
        func_8004F24C(1);
        s2 = 1;
        if (D_800AC64A == D_800F4CD0)
            func_80034E14();
        else
            func_8009AEAC(1, 1, 0);
        D_800F4CA8 = 2;
        D_800F4BE8 = 8;
        D_800F4C5C = 0x123;
        D_800F4D2C = D_800AC64A;
        v = (short)D_800AC64A;
        switch (v) {
        case 0:
            D_800F4D44 = 1;
            break;
        case 1:
            D_800F4D44 = 0;
            D_800FFC10 = 0;
            break;
        case 2:
            func_8001F290();
            D_800F4D44 = 2;
            D_800FFC10 = 0;
            break;
        case 3:
        case 4:
            D_800F4D44 = 0;
            break;
        case 5:
            D_800F4CA8 = 3;
            break;
        case 6:
            break;
        default:
            if (v == D_800F4CD0) {
                func_8001F290();
                D_800F4D44 = 4;
                D_800F4D2C = 8;
                D_800F4C5C = 0x11D;
            } else {
                D_800F4D2C = 7;
                D_800F4D44 = 6;
            }
            break;
        }
    } else {
        if (s1 == D_800F4CD0 + 1) {
            if (((func_8009AEAC(0, 0, 0) >> 7) & 3) == 2) {
                D_800F4D80[D_800AC64A].b0C = 0;
                s2 = 1;
                D_800F4D80[(short)D_800F4D2C].b0C = 0;
                func_8001F290();
                D_800F4D2C = 8;
                D_800F4D44 = 4;
                D_800F4C5C = 0x11D;
            }
        }
    }
    if (s2 == 1) {
        D_800F4D80[D_800AC64A].b0C = s2;
        for (i = 0; i < D_800F4BEC; i++) {
            if (D_800F4D80[i].b14 == 2) D_800F4D80[i].b14 = 3;
        }
    }
    return s2;
}
