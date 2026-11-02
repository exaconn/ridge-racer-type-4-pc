extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_8004D4BC();
extern int func_80050BCC();
extern int D_800A0224;
extern char *D_8010C1F0;
extern struct { int a, b, c, d; } D_8010C200;

int func_8004DB94(int a0, int a1) {
    int v;
    if (a0 < 0) return 1;
    switch (D_800A0224) {
    case 0:
        v = func_8001292C(func_8001223C(2, a0 + 3, a0), D_8010C1F0);
        if (v != 0) {
            D_800A0224 += 1;
    case 1:
            if (func_80012A58() != 0) {
                D_8010C200.c = (int)(D_8010C1F0 + *(int *)(D_8010C1F0 + 4));
                D_8010C200.a = (int)(D_8010C1F0 + *(int *)(D_8010C1F0 + 8));
                D_8010C200.b = (int)(D_8010C1F0 + *(int *)(D_8010C1F0 + 0xC));
                D_8010C200.d = a1;
                D_8010C1F0 += func_8004D4BC(*(int *)(D_8010C1F0 + 0xC));
                D_800A0224 += 1;
            }
            v = 0;
        }
        return v;
    case 2:
        func_80050BCC(&D_8010C200);
        D_800A0224 = 0;
        return 1;
    default:
        return 0;
    }
}
