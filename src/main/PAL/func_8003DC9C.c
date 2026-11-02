typedef struct { short f0, f2, f4, f6, f8, fA, fC, fE; } E;
extern int D_800AC540;
extern E *D_800ACC00;

void func_8003DC9C(int i, short a, short b, short c, short d, short e, short f) {
    if (i < D_800AC540) {
        E *p = (E *)((i << 4) + (int)D_800ACC00);
        p->f0 = f;
        p->f2 = 0;
        p->f4 = a;
        p->f6 = b;
        p->f8 = c;
        p->fA = d;
        p->fC = e;
    }
}
