typedef struct { short s[8]; short t[2]; short u[2]; } T;
extern T D_8010C170[];
extern char D_80011014[], D_80011030[];
extern int func_8001E84C();
void func_8004B738(unsigned a, unsigned b, short c, short d) {
    T *t;
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
