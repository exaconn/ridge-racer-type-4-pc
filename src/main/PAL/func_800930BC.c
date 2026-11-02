/* CC1_FLAGS: -mno-split-addresses */
typedef struct { int w; int s4; } A;
typedef struct { int a, b, c; } B;
extern int func_80092DB8();
extern char D_80011EFC[];
extern B D_800ABB60;
extern int *D_800ABAB8;
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
