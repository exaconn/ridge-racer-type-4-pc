/* GCC_VERSION: 2.7.2 */
struct R { int a, b; };
extern struct R *D_800ABCC8;
extern int (*D_800ABC90)();
int func_80097E38() {
    struct R *r = D_800ABCC8;
    if (!(r->b & 1)) return 0;
    if (!(r->a & 1)) return 0;
    if (D_800ABC90) D_800ABC90();
    return 1;
}
