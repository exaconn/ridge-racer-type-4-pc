extern void func_80043608();

void func_800436E8(int *p) {
    int n;
    p++;
    while ((n = *p++) > 0) {
        char *q = (char *)p + ((unsigned)n >> 2 << 2);
        func_80043608(p);
        p = (int *)q;
    }
}
