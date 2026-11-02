typedef struct { int f0, f4, f8, fC, f10, f14; } T;
int func_8004D420(T *t, int i, int *out) {
    int half;
    int v;
    if (i >= t->f4) {
        *out = -(t->f14 / 2);
        return -1;
    }
    half = t->f4 / 2;
    if (i < half) {
        v = t->f8 + t->f10 / 2 + t->f10 * i;
    } else {
        i -= half;
        v = t->fC + t->f14 / 2 + t->f14 * i;
    }
    *out = v;
    return 0;
}
