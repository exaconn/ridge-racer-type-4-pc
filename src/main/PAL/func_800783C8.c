extern char D_8010ECD0[];
int func_800783C8(int a0) {
    int a2 = 0, a1 = 0;
    char *v1;
    char *p = D_8010ECD0;
    int v;
    if (a0 > 0) { v1 = p; do { a1 += *(int *)(v1 + 0x18); v1 += 0x28; } while (++a2 < a0); }
    v = a1;
    if (v < 0) v += 0x1FFF;
    a1 = v >> 13;
    return 15 - a1;
}
