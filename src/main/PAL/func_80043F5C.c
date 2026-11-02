extern void func_80090EC4();
extern void func_80091D64();

void func_80043F5C(short *a, short b, int c) {
    int m[8];
    unsigned short v[3];
    short r[3];
    v[0] = a[0];
    v[1] = a[1];
    v[2] = 0;
    r[0] = 0;
    r[1] = 0;
    r[2] = -b;
    func_80091D64(v, m);
    func_80090EC4(m, r, c);
}
