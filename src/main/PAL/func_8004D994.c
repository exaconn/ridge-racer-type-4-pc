/* RODATA 0x80011068 0x20 */
extern int D_800A0220, D_8010C1F0;
extern short D_800F4D44, D_8009FE60[];
extern int func_8001223C(), func_8001292C(), func_80012A58(), func_8004D4BC(), func_80070108(), func_800436E8();
int func_8004D994(a0, a1) int a0, a1;
{
    int v, t;
    switch (D_800A0220) {
    case 0:
        if (D_800F4D44 != 1) return 1;
        v = func_8001292C(func_8001223C(2, a1 * 2 + 0x7D), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
        goto c1;
    case 2:
        v = func_8001292C(func_8001223C(2, a1 * 2 + 0x7C), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
    case 3:
        t = func_80012A58();
        if (t != 0) {
            func_80070108(D_8010C1F0, 8);
            v = func_8004D4BC(t);
            D_8010C1F0 += v;
            if (D_8009FE60[a0] < 7) {
                D_800A0220 = 0;
                return 1;
            }
            goto inc;
        }
        return 0;
    case 4:
        v = func_8001292C(func_8001223C(2, 0x85), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
    case 1:
    case 5:
    c1:
        t = func_80012A58();
        if (t != 0) {
            func_800436E8(D_8010C1F0);
        inc:
            D_800A0220++;
        }
        return 0;
    case 6:
        v = func_8001292C(func_8001223C(2, 0x84), D_8010C1F0);
        if (v == 0) goto out;
        D_800A0220++;
    case 7:
        t = func_80012A58();
        if (t != 0) {
            func_80070108(D_8010C1F0, 9);
            v = func_8004D4BC(t);
            D_8010C1F0 += v;
            D_800A0220 = 0;
            return 1;
        }
        return 0;
    default:
        return 0;
    }
out:
    return v;
}
