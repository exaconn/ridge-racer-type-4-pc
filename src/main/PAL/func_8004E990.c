extern int D_800A021C;
extern int D_800A0270[];
extern int *D_800ACC18;
extern int *D_8010C1F0;
extern int D_8010C210[];
extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_8004ED38();
extern int func_800436E8();

int func_8004E990()
{
    int r;
    int *q;
    switch (D_800A021C) {
    case 0:
        r = func_8001292C(func_8001223C(2, 0), D_8010C1F0);
        if (r != 0) {
            D_800A021C++;
    case 1:
            if (func_80012A58()) {
                q = D_800A0270;
                D_8010C210[0] = (int)D_8010C1F0 + D_8010C1F0[1];
                D_8010C210[1] = (int)D_8010C1F0 + D_8010C1F0[2];
                func_8004ED38(D_8010C210, q[1], q[2]);
                func_800436E8((int)D_8010C1F0 + D_8010C1F0[3]);
                D_800A021C = 0;
                D_8010C1F0 = (int *)((int)D_8010C1F0 + D_8010C1F0[2]);
                D_800ACC18 = D_8010C1F0;
                return 1;
            }
            r = 0;
        }
        break;
    default:
        r = 0;
    }
    return r;
}
