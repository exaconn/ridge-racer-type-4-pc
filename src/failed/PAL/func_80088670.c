extern unsigned short D_800A5128[], D_800A5140[];
unsigned func_80088670(a0, a1, a2, a3) int a0; int a1; int a2; int a3; {
    int s;
    int v;
    int t;
    short k;
    int o;
    int t0;
    short w;
    int q;
    short e;
    short r;
    int n;
    unsigned u;
    s = (short)((a3 & 0xFF) + a1);
    v = s / 128;
    t = a0 + v - (a2 & 0xFF);
    k = t;
    o = s - (v << 7);
    t0 = o;
    if ((short)o < 0) {
        t0 = (short)(o + 0x80);
        w = t0;
        k = t - 1 + (w / 128);
    }
    q = (short)k / 12;
    e = q - 2;
    r = (short)k % 12;
    if (r < 0) {
        r = r + 12;
        e = q - 3;
    }
    if (e >= 0) {
        u = 0x3FFF;
    } else {
        n = -e;
        u = (((D_800A5128[r] * D_800A5140[t0]) >> 16) + (1 << (n - 1))) >> n;
    }
    return u & 0xFFFF;
}
