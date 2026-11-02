extern int D_8010718C;
extern short D_800F4C58;
extern int func_8006F188();
extern int func_80026A20();
int func_8006F1EC(a0)
int a0;
{
    int t, n;
    if (D_8010718C == 0) {
        t = func_8006F188(a0);
        n = *(int *)(a0 + 0x14);
        a0 = (int)&D_800F4C58;
        func_80026A20(t, n, a0);
        return *(short *)a0;
    }
    return func_8006F188() / 5;
}
