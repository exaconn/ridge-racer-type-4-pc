typedef struct {
    int f0, f4, f8, fC, f10, f14, f18, f1C, f20, f24, f28, f2C, f30, f34, f38, f3C, f40, f44;
    int t48[16];
    int t88[16];
    int fC8, fCC, fD0, fD4, fD8, fDC, fE0, fE4, fE8, fEC, fF0, fF4;
} Q;
extern int func_8008B154();
extern int func_80096474();
extern short D_80107180[];
extern int D_8010E738;
extern int D_8010E740[];
extern int D_8010E768[];
extern char *D_8010E78C;

void func_80070934(int a0, int a1, int *a2, int a3) {
    Q *t = (Q *)(D_8010E78C + a0 * 0xF8);
    int f;
    int i;
    int *p;
    if (a1 & 1) {
        if (a2 != 0) {
            t->fC8 = a2[0];
            t->fCC = a2[1];
            t->fD0 = a2[2];
        }
    }
    if (a1 & 2) {
        if (a3 >= 0) t->f3C = a3;
    }
    f = a1 & 4;
    a1 = 0;
    if (f) {
        t->f8 = 0xFF;
        t->fC = 5;
        t->f20 = 0x1000;
        t->f24 = 0x1000;
        t->f28 = 0x20;
        t->f2C = 0xB5;
        t->f30 = 1;
        t->f4 = 0;
        t->f10 = 0;
        t->f14 = 0;
        t->f18 = 0;
        t->f1C = 0;
        t->f34 = 0;
        t->f38 = 0;
        t->f0 = D_8010E738 + a0 * 0x23DC;
        t->f40 = D_8010E740[t->f3C];
        t->f44 = D_8010E768[t->f3C];
        do {
            t->t48[a1] = 0x52000000;
            t->t88[a1] = 0x52000000;
            a1++;
        } while (a1 < 0x10);
        t->fD8 = 0;
        t->fDC = 0;
        t->fE0 = 0;
        t->fF0 = 0;
        t->fF4 = 0;
        D_80107180[0] = 0x420;
        D_80107180[1] = 0x5D0;
        D_80107180[2] = 0x610;
        func_80096474(func_8008B154(1, a1));
    }
}
