extern char *D_800ACC14;
extern unsigned short D_8009F01C[];
extern int func_8002BB88();
extern int func_80020E2C();
void func_8002BD18(int a, int b)
{
    char *p = D_800ACC14 + (b * 120 + 0x226EC);
    char *q;
    int r;
    int i;
    int t;
    if (b == 0) {
        q = D_800ACC14 + 0x70;
    } else {
        q = D_800ACC14 + 0xB70;
    }
    r = func_8002BB88(a);
    i = a < 0x5DC;
    t = D_8009F01C[i];
    func_80020E2C(q, 0x111, *(short *)(p + 0xA), r, t);
}
