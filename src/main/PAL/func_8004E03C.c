extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_8004ED38();
extern int D_800A021C;
extern int D_800A0228[];
extern char *D_8010C1F0;
extern int D_8010C210[2];

int func_8004E03C(char *a0) {
    int v;
    int *t = D_800A0228;
    switch (D_800A021C) {
    case 0:
        v = func_8001292C(func_8001223C(2, 0x11), a0);
        if (v != 0) {
            D_800A021C += 1;
    case 1:
            if (func_80012A58() != 0) {
                D_800A021C += 1;
            }
            v = 0;
        }
        return v;
    case 2:
        v = func_8001292C(func_8001223C(2, 0x12), D_8010C1F0);
        if (v != 0) {
            D_800A021C += 1;
    case 3:
            if (func_80012A58() != 0) {
                D_8010C210[0] = (int)a0;
                D_8010C210[1] = (int)D_8010C1F0;
                func_8004ED38(D_8010C210, t[1], t[2]);
                D_800A021C = 0;
                return 1;
            }
            v = 0;
        }
        return v;
    default:
        return 0;
    }
}
