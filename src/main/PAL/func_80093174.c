/* CC1_FLAGS: -mno-split-addresses */
extern char D_80011F08[];
extern int (*D_800ABABC)();
extern unsigned char D_800ABAC2;
extern char D_800ABB6C[];
extern int D_800ABB80;
int *func_80093174(a0, a1) int *a0; int a1; {
    /* ASM_BARRIER: pins p->$16, n->$17 to match original s-reg assignment */
    register int *p asm("$16") = a0;
    int n = a1;
    int *q; int *t; int m; int (*f)();
    if (D_800ABAC2 >= 2) { f = D_800ABABC; q = (int *)D_80011F08; f(q, p, n); }
    n--;
    if (n != 0) {
        do {
            n--;
            q = p + 1;
            ((char *)p)[3] = 0;
            m = (int)q & 0xFFFFFF; *p = (*p & 0xFF000000) | m;
            p = q;
        } while (n != 0);
    }
    t = &D_800ABB80;
    *t = ((int)D_800ABB6C & 0xFFFFFF) | 0x04000000;
    *p = (int)t & 0xFFFFFF;
    return p;
}
