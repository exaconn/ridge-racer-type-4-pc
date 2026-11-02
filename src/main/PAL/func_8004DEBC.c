extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_8004D4BC();
extern int func_8004ED38();
extern int D_800A0224;
extern int D_800A0228[];
extern struct { char pad[0x2858]; char *p[1]; } D_800F4DB8;
extern char *D_8010C1F0;
extern char *D_8010C210[2];
extern int D_8010C218;

int func_8004DEBC(int a0, int a1) {
    int s0 = 0;
    int *s2 = D_800A0228 + a0 * 3;
    int v, t;
    char *v1, *v0;
    switch (D_800A0224) {
    case 0:
        v = func_8001292C(func_8001223C(2, s2[0]), D_8010C1F0);
        if (v != 0) {
            D_800A0224 += 1;
    case 1:
            if (func_80012A58() != 0) {
                if (a1 > 0) s0 += a1;
                v1 = (char *)(s0 * 4 + (int)D_8010C1F0);
                D_8010C210[0] = D_8010C1F0 + *(int *)(v1 + 4);
                v0 = D_8010C1F0 + *(int *)(v1 + 8);
                D_8010C210[1] = v0;
                s0 = v0 - D_8010C1F0;
                func_8004ED38(D_8010C210, s2[1], s2[2], D_8010C1F0);
                if (a1 > 0) {
                    D_800F4DB8.p[D_8010C218] = D_8010C1F0 + *(int *)(D_8010C1F0 + 4);
                }
                s0 = func_8004D4BC(s0);
                D_800A0224 = 0;
                D_8010C1F0 += s0;
                return 1;
            }
            v = 0;
        }
        return v;
    default:
        return 0;
    }
}
