extern int func_80017CC0();
extern int func_80026998();
extern int D_800ABFE8, D_800AC308, D_8010718C;
void func_80036FD4(int a0) {
    int v;
    if (D_8010718C == 1) {
        if (a0 < 2) {
            v = D_800ABFE8;
        } else {
            v = D_800AC308;
        }
    } else {
        int *p = (int *)0x1F800000;
        v = func_80026998(p[2], p[4]);
    }
    func_80017CC0(v);
}
