extern void func_8006E1AC();
extern void func_80090EC4();
extern void func_800912D4();
extern void func_80091D64();

void func_800451E4(int *a) {
    int m[8];
    unsigned short v[3];
    short r[3];
    int t[3];
    int *s = (int *)0x1F800000;
    s[6] = -*(short *)((char *)a + 0x10);
    s[7] = *(short *)((char *)a + 0x12);
    s[8] = *(short *)((char *)a + 0x14);
    v[0] = *(short *)((char *)a + 0x10);
    v[1] = *(short *)((char *)a + 0x12);
    v[2] = 0;
    r[0] = 0;
    r[1] = 0;
    r[2] = -*(unsigned short *)((char *)a + 0x1C);
    func_80091D64(v, m);
    func_80090EC4(m, r, t);
    s[2] = a[0] + t[0];
    s[3] = a[1] + t[1];
    s[4] = a[2] + t[2];
    func_800912D4(a[8]);
    func_8006E1AC();
}
