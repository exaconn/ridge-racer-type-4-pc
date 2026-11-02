/* GCC_VERSION: 2.7.2 */
extern int func_80096054(), func_80096434();
struct B { char c[32]; };
struct W { char c[4]; };
struct E { int a, b; short c; char pad[22]; };
extern struct E D_801140F8[];
extern char D_801142D8[];
extern char D_80114328[];
int func_80096198(a0) int a0; {
    int i;
    int o;
    int r;
    struct B *pp;
    int k1;
    int ka;
    int kf;
    struct B *p;
    char *q;
    char *buf;
    char *b2;
    int m1;
    int one;
    i = 0;
    buf = D_80114328;
    p = (struct B *)D_801140F8;
    ka = 0xA0;
    kf = 0xFFFF;
    k1 = 1;
    for (;;) {
        func_80096434(buf, 0x80);
        pp = (struct B *)((i << 5) + (int)p);
        func_80096434(pp, 0x20);
        D_801140F8[i].a = ka;
        D_801140F8[i].b = 0;
        D_801140F8[i].c = kf;
        *(struct B *)buf = *pp;
        o = i + 1;
        r = func_80096054(a0, o, buf);
        if (r == k1) {
            i = o;
            if (i < 0xF) continue;
            break;
        }
        return 0;
    }
    i = 0;
    m1 = -1;
    b2 = buf;
    one = 1;
    q = D_801142D8;
    for (;;) {
        *(int *)q = m1;
        func_80096434(b2, 0x80);
        *(struct W *)b2 = *(struct W *)q;
        r = func_80096054(a0, i + 0x10, b2);
        i++;
        if (r == one) {
            q += 4;
            if (i < 0x14) continue;
            func_80096434(b2, 0x80);
            b2[0] = 0x4D;
            b2[1] = 0x43;
            return func_80096054(a0, 0, b2) == 1;
        }
        return 0;
    }
}
