typedef struct { char pad[0x54]; int f54; char pad2[4]; } T;

typedef struct { short s[8]; short t[2]; short u[2]; } T_1;

typedef struct { short s0, s2, s4, s6, s8, sA, sC, sE, s10, s12, s14, s16; } T_2;

typedef struct { short s0, s2, s4, s6, s8, sA, sC, sE, s10, s12, s14, s16; } T_3;

extern T D_800F3A38[];
extern T_1 D_8010C170[];
extern char D_80011014[], D_80011030[];
extern int func_8001E84C();

int func_8004B70C(int i);
void func_8004B738(unsigned a, unsigned b, short c, short d);
void func_8004B7E0(int i, int k, int v);
void func_8004B864(int i);

int func_8004B70C(int i) {
    return D_800F3A38[i].f54;
}

void func_8004B738(unsigned a, unsigned b, short c, short d) {
    T_1 *t;
    if (a >= 2) {
        func_8001E84C(D_80011014, a);
    }
    if (b >= 2) {
        func_8001E84C(D_80011030, b);
    }
    t = &D_8010C170[a];
    t->t[b] = d;
    t->s[b] = c;
    t->u[0] = 0;
    t->u[1] = 0;
}

void func_8004B7E0(int i, int k, int v) {
    T_2 *t = &D_8010C170[i];
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

void func_8004B864(int i) {
    T_3 *t = &D_8010C170[i];
    t->s10 = 0;
    t->s0 = 0;
    t->s12 = 0;
    t->s2 = 0;
    t->s4 = 0;
    t->s6 = 0;
    t->s8 = 0;
    t->sA = 0;
    t->sC = 0;
    t->sE = 0;
}
