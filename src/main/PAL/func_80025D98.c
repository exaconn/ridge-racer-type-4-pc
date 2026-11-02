#define W(p,o) (*(int *)((char *)(p) + (o)))
#define H(p,o) (*(short *)((char *)(p) + (o)))
extern char *D_800ABEA4;
extern int D_800ABECC;
void func_80025D98(a, r, c) int a; int *r; int c; {
    int n = (a + 1) % D_800ABECC;
    int c2 = c;
    int t = 0x400 - c2;
    char *p = D_800ABEA4 + a * 0x3C;
    char *q = D_800ABEA4 + n * 0x3C;
    r[0] = (t * W(p,8) + c2 * W(q,8)) / 1024;
    r[1] = (t * H(p,0x10) + c2 * H(q,0x10)) / 1024;
    r[2] = (t * W(p,0xC) + c2 * W(q,0xC)) / 1024;
}
