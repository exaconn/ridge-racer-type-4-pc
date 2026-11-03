/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
typedef struct { int w; int s4; } A;

typedef struct { int a, b, c; } B;

extern int func_80092DB8();
extern char D_80011EFC[];
extern B D_800ABB60;
extern int *D_800ABAB8;
extern char D_80011F08[];
extern int (*D_800ABABC)();
extern unsigned char D_800ABAC2;
extern char D_800ABB6C[];
extern int D_800ABB80;

int func_800930BC();
int *func_80093174();

int func_800930BC(a0, a1, a2)
A *a0; int a1; int a2;
{
    B *p;
    int *o;
    int w;
    func_80092DB8(D_80011EFC, a0);
    if (*(short *)&a0->s4 == 0) return -1;
    if (*((short *)&a0->s4 + 1) != 0) {
    p = &D_800ABB60;
    w = a0->w;
    o = D_800ABAB8;
    p->b = (a2 << 16) | (a1 & 0xFFFF);
    p->a = w;
    p->c = a0->s4;
    return ((int (*)())o[2])(o[6], (char *)p - 8, 0x14, 0);
    }
    return -1;
}

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
