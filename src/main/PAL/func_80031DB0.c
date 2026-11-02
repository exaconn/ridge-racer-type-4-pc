struct S { int a; short b; short c; };
extern struct S D_800F2A68[];
extern struct S *D_800F2DE8[];
void func_80031DB0() {
    int i = 0;
    struct S **q = D_800F2DE8;
    struct S *p = D_800F2A68;
    do { p->a = 0; p->b = i; *q++ = p; i++; p++; } while (i < 8);
}
