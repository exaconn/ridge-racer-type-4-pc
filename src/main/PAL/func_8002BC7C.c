extern char *D_800ACC14;
extern int D_800F3A30;
extern unsigned short D_8009F01C[];
extern int func_8002BB88();
extern int func_80020E2C();
void func_8002BC7C(int a)
{
    char *p = D_800ACC14 + 0x226EC;
    int r = func_8002BB88(a);
    int i = a < 0x5DC;
    int t = D_8009F01C[i];
    int s = (D_800F3A30 - 3) * 10 + 0x50;
    func_80020E2C(D_800ACC14 + 0x70, 0x111, s, r, t);
    *(short *)(p + 0xA) = s;
}
