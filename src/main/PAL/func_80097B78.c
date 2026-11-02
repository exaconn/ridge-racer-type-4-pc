struct E { unsigned char n; char pad[3]; unsigned char *p; };
struct S { char pad[8]; struct E *e; char pad2[0xde]; unsigned char c; };
extern struct S *(*D_800ABC7C)();
int func_80097B78(a0, a1, a2) int a0, a1, a2; {
    struct S *s = D_800ABC7C();
    struct E *e;
    if (a1 < 0) return s->c;
    if (a1 >= s->c) return 0;
    e = s->e + a1;
    if (a2 < 0) return e->n;
    if (a2 >= e->n) return 0;
    return e->p[a2];
}
