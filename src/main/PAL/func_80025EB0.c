extern int func_80025C94();
extern int func_80025D0C();
extern int D_800ABECC;
void func_80025EB0(int a, int b)
{
    int s2;
    int s0;
    int x;
    s2 = func_80025D0C(a, b);
    x = a - 2;
    if (x < 0) {
        int d = D_800ABECC - 2;
        x = d + a;
    }
    s0 = func_80025C94(s2, func_80025D0C(x, b), 0x200);
    func_80025C94(s0, func_80025C94(s2, func_80025D0C((a + 2) % D_800ABECC, b), 0x200), 0x200);
}
