typedef struct { short s0, s2, s4, s6, s8, sA, sC, sE, s10, s12, s14, s16; } T;
extern T D_8010C170[];
void func_8004B7E0(int i, int k, int v) {
    T *t = &D_8010C170[i];
    switch (k) {
    case 0:
        t->s4 = 0;
        return;
    case 1:
        t->s4 = v + 1;
        t->s8 = 0;
        t->sC = 0;
        t->s14 = 0;
        return;
    case 2:
        t->s6 = v + 1;
        t->sA = 0;
        t->sE = 0;
        t->s14 = 0;
        return;
    }
}
