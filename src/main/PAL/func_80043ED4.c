extern void func_80090EC4();
extern void func_80091D64();

void func_80043ED4(short *a, short b, int c) {
    int m[8];
    unsigned short v[3];
    short r[3];
    int *s = (int *)0x1F800000;
    s[6] = -a[0];
    s[7] = a[1];
    s[8] = a[2];
    v[0] = a[0];
    v[1] = a[1];
    v[2] = 0;
    r[0] = 0;
    r[1] = 0;
    r[2] = -b;
    func_80091D64(v, m);
    func_80090EC4(m, r, c);
}
