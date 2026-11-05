typedef struct { char pad[0x320]; } E;

extern int func_800373DC();

extern int func_80022980();

extern int func_80045F48();

extern int func_800293D0();

extern int func_80096434();

extern E D_800ABEF8[];

extern short D_800AC3FC;

extern int D_800ABEC4;

extern short D_800AC59C;

extern signed char D_800ABEBC[];

extern int CarTable[];

extern int D_800F2EB8;

extern short D_80107178;

extern int D_800ADAC8;

extern int D_800F4C64;

void func_80037B3C(int a0)
{
    int s1, s2, i, t;
    signed char *s3;
    int *s0;
    D_800AC3FC = 0;
    func_800373DC();
    D_800ABEC4 = 0;
    switch (a0) {
    case 0:
    default:
        i = 0;
        func_80022980(D_800ABEF8, i, i);
        func_800293D0(0, 0);
        break;
    case 1:
        func_80022980(D_800ABEF8, 0, 7);
        func_80045F48(7);
        func_800293D0(0, 0);
        break;
    case 2:
        i = 0;
        func_80022980(D_800ABEF8, i, i);
        i = 1;
        func_80022980(D_800ABEF8 + 1, i, i);
        func_800293D0(1, 0);
        break;
    case 3:
        func_80022980(D_800ABEF8, 0, D_800AC59C - 2);
        func_80022980(D_800ABEF8 + 1, 1, D_800AC59C - 1);
        func_80045F48(D_800AC59C - 2);
        func_800293D0(1, 0);
        break;
    case 4:
        s1 = 0;
        s2 = 0;
        if (D_800AC59C > 0) {
            s3 = D_800ABEBC;
            s0 = CarTable;
            do {
                if (*(signed char *)(s1 + (int)s3) == 1) {
                    func_80022980(*s0, s1, s2);
                    s2++;
                } else {
                    *(short *)(*(int *)s0 + 0x1E4) = 0;
                    i = *(int *)s0;
                    t = D_800F2EB8;
                    *(short *)(i + 0x1E8) = -1;
                    *(int *)(*(int *)s0 + 0x184) = -(t * 300);
                }
                s0++;
                s1++;
            } while (s1 < D_800AC59C);
        }
        D_80107178 = s2;
        func_800293D0(2, 0);
        break;
    case 5:
        func_80096434(D_800ABEF8, 0x320);
        func_80045F48(7);
        func_800293D0(1, 0);
        D_800ADAC8 = 2;
        break;
    case 6:
        func_80022980(D_800ABEF8, 0, 1);
        func_80045F48(1);
        func_800293D0(0, 0);
        break;
    }
    D_800F4C64 = 0;
}
