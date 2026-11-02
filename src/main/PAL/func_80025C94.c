int func_80025C94(int a0, int a1, int a2) {
    int d;
    a0 &= 0xFFF;
    a1 &= 0xFFF;
    d = 0x400 - a2;
    if (a1 < a0) {
        if (a0 - a1 > 0x800) a1 += 0x1000;
    } else {
        if (a1 - a0 > 0x800) a0 += 0x1000;
    }
    return ((a0 * d + a1 * a2) / 1024) & 0xFFF;
}
