extern int func_8004FB00();
typedef struct { short pad[0xC]; short o; short n; short c; short to; short from; } T;
extern T D_800F4DB8;

void func_8004FCB4(void) {
    T *p = &D_800F4DB8;
    int n = p->n;
    int v = 0x80;
    int c;
    short c1;
    if (n > 0) {
        c = (unsigned short)p->c;
        if (p->c < n) {
            n = ((p->to - p->from) << 12) / n;
            c1 = c + 1;
            p->c = c1;
            v = (unsigned short)p->from + n * c1 / 4096;
        } else {
            p->n = 0;
            func_8004FB00(n, p, c);
            return;
        }
    }
    p->o = v;
}
