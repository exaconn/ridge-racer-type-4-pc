extern void func_80056F04();
void func_80057138(int a0, int a1) {
    unsigned short sp[2];
    int s0, s1;
    int s2;
    int s3;
    int d1, d2, d3, d4;
    s2 = a0;
    if (s2 < 0x10) {
        s0 = 0x10 - s2;
        sp[0] = 0x80 - s0;
    } else {
        s0 = 0;
        sp[0] = 0x80;
    }
    sp[1] = 0xD8;
    if (a1 < s2) {
        s1 = 0x80 - ((s2 - a1) * 4);
    } else {
        s1 = 0x80;
    }
    s3 = 0;
    d1 = s0 + 0x10;
    d2 = s0 - 0x36;
    d3 = s0 - 0x10;
    d4 = s0 + 0x36;
    do {
        func_80056F04(0, (short)(sp[0] - d1), (short)(sp[1] + d2), s1, 0);
        func_80056F04(1, (short)(sp[0] + d3), (short)(sp[1] - d4), s1, 0);
        s3++;
        sp[0] += 0x20;
        sp[0] += s0;
    } while (s3 < 3);
    if (s2 >= 0x10) {
        s2 -= 0x10;
        if (s2 < 0x10) s1 = s2 * 8;
        func_80056F04(2, 0, 0x110, s1, 1);
    }
}
