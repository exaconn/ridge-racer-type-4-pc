int func_80090604(a0) int a0; {
    int buf[16];
    int *s;
    int i;
    int v, a, b;
    int *p;
    int *q;
    s = buf + 1;
    s[0] = a0 + 0x5D50AD;
    s[8] = a0 - 0x5D50AD;
    i = 1;
    q = &s[9];
    p = &s[0];
    do {
        if (i != 4) {
            v = p[8];
            if (v >= 0) {
                p[1] = p[0] - (v >> i);
                *q = p[8] - (p[0] >> i);
            } else {
                p[1] = p[0] + (v >> i);
                (&s[8])[i] = p[8] + (p[0] >> i);
            }
        } else {
            v = s[11];
            if (v >= 0) {
                a = s[3] - (v >> 4);
                b = v - (s[3] >> 4);
            } else {
                a = s[3] + (v >> 4);
                b = v + (s[3] >> 4);
            }
            s[3] = a;
            s[11] = b;
            if (b >= 0) {
                s[4] = a - (b >> 4);
                s[13] = b - (a >> 4);
            } else {
                s[4] = a + (b >> 4);
                s[13] = b + (a >> 4);
            }
        }
        q++;
        i++;
        p++;
    } while (i < 7);
    return s[6];
}
