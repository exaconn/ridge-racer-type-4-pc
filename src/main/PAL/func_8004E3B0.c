typedef struct { unsigned char a, b, c, d; } T;
extern T D_800F4D98[];
int func_8004E3B0(int i) {
    T *base = D_800F4D98;
    T *t = base + i;
    int r = 0;
    if (t->c != 0x27) {
        r = t->b;
        if (t->c == 0x13) {
            r += 4;
        }
    } else if (t->b == 0) {
        r = (t->a == 2) * 8;
    }
    return r;
}
