extern void func_80092FFC();
extern void func_80092D50();

void func_80043608(int *p) {
    short r[4];
    unsigned short *h;
    p++;
    if (*p++ & 8) {
        h = (unsigned short *)p;
        r[0] = h[2];
        r[1] = h[3];
        r[2] = h[4];
        r[3] = h[5];
        func_80092FFC(r, h + 6);
        func_80092D50(0);
        p = (int *)((char *)h + (*(unsigned *)h >> 2 << 2));
    }
    h = (unsigned short *)p;
    r[0] = h[2];
    r[1] = h[3];
    r[2] = h[4];
    r[3] = h[5];
    if (r[2] > 0 && r[3] > 0) {
        func_80092FFC(r, h + 6);
        func_80092D50(0);
    }
}
