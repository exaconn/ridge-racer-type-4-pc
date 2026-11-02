short func_80043FC0(short *p, int t) {
    int n = p[0x24 / 2] * 5;
    int v;
    if (n > 0 && t < n) {
        v = (p[0x22 / 2] - p[0x20 / 2]) * t / n + p[0x20 / 2];
    } else {
        v = p[0x22 / 2];
    }
    return v - (v / 4096 * 4096);
}
