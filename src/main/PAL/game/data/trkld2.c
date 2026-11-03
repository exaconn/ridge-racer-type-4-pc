extern int D_800A0220;
extern int *D_8010C1F0;
extern struct { char pad[0x24C]; char *p; char pad2[0x560 - 0x250]; char *r560; } D_800AC650;
extern int func_8001223C(), func_8001292C(), func_80012A58(), func_8004D4BC(), func_80070108(), func_800436E8(), func_80043608(), func_80012A68();
int func_8004D7D8(a0, a1) int a0, a1;
{
    int v, t, e;
    e = 0x86;
    switch (D_800A0220) {
    case 0:
        v = func_8001292C(func_8001223C(2, (a0 * 4) + (a1 + e)), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
    case 1:
        t = func_80012A58();
        if (t != 0) {
            func_800436E8((char *)D_8010C1F0 + D_8010C1F0[1]);
            func_80043608((char *)D_8010C1F0 + D_8010C1F0[2]);
            D_800A0220++;
        }
        return 0;
    case 2:
        v = func_8001292C(func_8001223C(2, 0x96), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
    case 3:
        t = func_80012A58();
        if (t != 0) {
            func_80070108((char *)D_8010C1F0 + D_8010C1F0[1], 8);
            func_80012A68((char *)D_8010C1F0 + D_8010C1F0[2]);
            D_800AC650.r560 = (char *)D_8010C1F0 + D_8010C1F0[2];
            ((int *)D_800AC650.p)[0x138 / 4] = (int)((char *)D_8010C1F0 + D_8010C1F0[3]);
            v = func_8004D4BC(t);
            D_800A0220 = 0;
            D_8010C1F0 = (int *)((char *)D_8010C1F0 + v);
            return 1;
        }
        return 0;
    default:
        return 0;
    }
out:
    return v;
}
