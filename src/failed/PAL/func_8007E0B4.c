extern int func_8007DC04();
extern int func_8007E9C4();

int func_8007E0B4(a0, a1)
int a0;
int a1;
{
    int sp[2];
    int s2, s3, v1;

    if ((a1 & 0x7FFFFFFF) == 0 && a0 == 0) return 0;
    s2 = (a1 >> 20) & 0x7FF;
    s3 = s2 - 0x41D;
    if (s3 > 0) {
        func_8007E9C4(0x22, 0x11);
        if (a1 >= 0) return 0x7FFFFFFF;
        return 0x80000000;
    }
    sp[1] = (a1 & 0xFFFFF) | 0x100000;
    sp[0] = a0;
    func_8007DC04(sp, 0, sp[0], sp[1], 0xA);
    v1 = sp[1];
    if ((unsigned)(s2 - 0x3FE) < 0x20 && v1 != 0) {
        v1 = v1 >> -s3;
        if (a1 >= 0) return v1;
        return -v1;
    }
    return 0;
}
