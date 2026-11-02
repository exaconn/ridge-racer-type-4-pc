struct E { int a; short b; short c; };
extern short D_800AC59C;
extern struct E D_800F2A68[];
extern struct E *D_800F2DE8[];

void func_80031DEC(void) {
    int i = 0;
    struct E *e;
    struct E **t;
    int n;
    if (D_800AC59C > 0) {
        n = D_800AC59C;
        t = D_800F2DE8;
        e = D_800F2A68;
        do {
            e->a = 0;
            e->b = i;
            *t = e;
            t++;
            i++;
            e++;
        } while (i < n);
    }
}
